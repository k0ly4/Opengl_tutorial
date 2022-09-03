#ifndef TYPE_OBJECT_H
#define TYPE_OBJECT_H
#include "Shader.h"
#include "Texture.h"

class Transformable2D {
protected:
	float rotate;
	glm::vec2 scale;
	glm::vec2 position;
	glm::vec2 origin;
public:
	virtual void setPosition(glm::vec2) = 0;
	virtual void setScale(glm::vec2) = 0;
	virtual void setOrigin(glm::vec2) = 0;
	virtual void setRotate(float) = 0;
	Transformable2D() :scale(1.f), position(0.f), origin(0.f), rotate(0.f) {}

	inline void Move(glm::vec2 offset) {
		setPosition(position + offset);
	}
	inline void Scale(glm::vec2 offset) {
		setScale(scale + offset);
	}
	inline void Rotate(float offset) {
		setRotate(rotate + offset);
	}
};
class Transformable :public Transformable2D {
protected:
	bool need_update_model = 0;
public:
	inline void setPosition(glm::vec2 position) {
		this->position = position;
		need_update_model = 1;
	}
	inline void setRotate(float angle) {
		rotate = angle;
		need_update_model = 1;
	}
	inline void setOrigin(glm::vec2 origin) {
		this->origin = origin;
		need_update_model = 1;
	}
	inline void setScale(glm::vec2 scale) {
		this->scale = scale;
		need_update_model = 1;
	}

	void setPosition(float x, float y) {
		setPosition(glm::vec2(x, y));
	}
	void setScale(float x, float y) {
		setScale(glm::vec2(x, y));
	}
	void setOrigin(float x, float y) {
		setOrigin(glm::vec2(x, y));
	}

};

struct Angle3D {
	glm::vec3 axis;
	float angle;
	Angle3D() :angle(0.f), axis(0.f) {}
	Angle3D(float Angle, const glm::vec3& Axis) :angle(Angle), axis(Axis) {}
	Angle3D(const glm::vec4& rotate) :axis(rotate.x, rotate.y, rotate.z), angle(rotate.w) {}
};
struct Transform3D {
	Angle3D rotate;
	glm::vec3 position;
	glm::vec3 scale;
	Transform3D() :position(0.f), scale(1.f), rotate(0.f, glm::vec3(0.f, 1.f, 0.f)) {}
	Transform3D(const glm::vec3& Pos, const glm::vec3& Scale, const Angle3D& Rotate) :rotate(Rotate), position(Pos), scale(Scale) {}
	glm::mat4 getModel() {
		return glm::scale(glm::rotate(glm::translate(glm::mat4(1.f), position), glm::radians(rotate.angle), rotate.axis), scale);
	}
};

class Transformable3D {
	bool needUpMatrix = 1;
	glm::mat4 model;
	inline void setupModel() {
		if (needUpMatrix) {
			model = glm::mat4(1.f);
			model = glm::translate(model, transform.position);
			model = glm::rotate(model, glm::radians(transform.rotate.angle), transform.rotate.axis);
			model = glm::scale(model, transform.scale);
			needUpMatrix = 0;
		}
	}
	Transform3D transform;
protected:
	inline void uniformTransform(const Shader& shader) {
		shader.uniform("model", getModel());
	}
	const glm::mat4& getModel() {
		setupModel();
		return model;
	}
public:
	 const Transform3D& getTransform()const  {
		return transform;
	}
	 const glm::vec3& getPosition()const {
		return transform.position;
	}
	 const  glm::vec3& getScale()const {
		return transform.scale;
	}
	 const  Angle3D& getRotate()const {
		return transform.rotate;
	}
	 
	 void setTransform(const Transform3D& _transform) {
		 transform = _transform;
		 needUpMatrix = 1;
	 }
	void setPosition(const glm::vec3& Position) {
		transform.position = Position;
		needUpMatrix = 1;
	}
	void setScale(const glm::vec3& Scale) {
		transform.scale = Scale;
		needUpMatrix = 1;
	}
	void setRotate(const Angle3D& Angle) {
		transform.rotate = Angle;
		needUpMatrix = 1;
	}
	Transformable3D() : model(1.f) {
		transform.position = glm::vec3(0.f);
		transform.scale = glm::vec3(1.f);
		transform.rotate = Angle3D(0.f, glm::vec3(0.f, 1.f, 0.f));
	}

	inline void Move(const glm::vec3& offset) {
		setPosition(transform.position + offset);
	}
	inline void Scale(const glm::vec3& offset) {
		setScale(transform.scale + offset);
	}
	inline void Rotate(float offset) {
		setRotate(Angle3D(transform.rotate.angle + offset, transform.rotate.axis));
	}
};
//Интерфейс между RenderGeneral* и объектом
class Drawable {
public:
	glShader::Object id_obj;
	virtual void draw(View* view, const Shader& shader) = 0;
};
class gbMateriable:public Drawable {
protected:
	float specular = 0.1f;
	glm::vec3 base_color = glm::vec3(1.f);
	Texture2D* diffuse = 0;
	void uniformMaterial(const Shader& shader) {
		shader.uniform("specularMaterial", specular);
		if (diffuse) diffuse->use(0);
		else shader.uniform("color", base_color);
	}
public:
	void setSpecular(float _specular) {
		specular = _specular;
	}
	void setColor(const glm::vec3& _color) {
		base_color = _color;
		diffuse = 0;
		id_obj = glShader::gb_color_uniform;
	}
	void setTexture(Texture2D& texture) {
		id_obj = glShader::gb_texturable;
		this->diffuse = &texture;
	}
};
//Instance chapter
//Интерфейс для instance
class Instancable :public Drawable, public Transformable3D {
public:
	virtual const DrawBuffer& getDrawBuffer() = 0;
	virtual void uniform(const Shader& shader) = 0;
};
//Матрицы трансформации в instance массив
struct InstanceMatrix {
	glm::mat4 model = glm::mat4(1.f);
	Transform3D transform;
	virtual inline void setPosition(const glm::vec3& position) {
		transform.position = position;
	}
	inline void setRotate(const Angle3D& angle) {
		transform.rotate = angle;
	}
	inline void setScale(const glm::vec3& scale) {
		transform.scale = scale;
	}
	inline void saveModel() {
		model = transform.getModel();
	}
};
//Интерфейс для instance и render
class GeneralInstance :public Drawable {
protected:
	Instancable* object = 0;
	DrawBuffer VAO;
	std::vector<glm::mat4> matrix;
	VertexBufferObject m_VBO;
	void attibMatrix() {
		VAO.begin();
		m_VBO.begin();
		m_VBO.setMode(GL_DYNAMIC_DRAW);
		m_VBO.data(matrix);
		VAO.attribInstanceVBOMat4(VAO.sizeAttribute());
		VAO.end();
		m_VBO.end();
	}
	
public:
	const Instancable* getObject() {
		return object;
	}
	virtual void setObject(Instancable* Object, glShader::Object shader_configuraion) {
		id_obj = shader_configuraion;
		object = Object;
		VAO = object->getDrawBuffer();
	}
	virtual void draw(View* view, const Shader& shader) {
		shader.use();
		view->use(shader);
		object->uniform(shader);
		VAO.draw();
	}
};
class InstanceObject:public GeneralInstance {
public:
	inline glm::mat4& operator[](size_t index) {
		return matrix[index];
	}
	inline void upVBO() {
		m_VBO.begin();
		m_VBO.setMode(GL_DYNAMIC_DRAW);
		m_VBO.data(matrix);
	}
	virtual void create(const std::vector<glm::mat4>& models) {
		matrix = models;
		attibMatrix();	
		VAO.data_draw=DataDraw(VAO.data_draw, models.size());
	}
	virtual void create(VertexBufferObject& buffer, size_t size) {
		VAO.begin();
		VAO.attribInstanceVBOMat4(VAO.sizeAttribute());
		VAO.data_draw = DataDraw(VAO.data_draw, size);
	}
};

#endif