#ifndef INSTANCABLE_H
#define INSTANCABLE_H

#include"Transformable.h"
#include "Drawable.h"

//Instance chapter
//Интерфейс для instance
class Instancable:public Drawable, public Transformable3D {
public:
	virtual const DrawBuffer& getDrawBuffer() = 0;
	virtual void uniform(const Shader & shader) = 0;
};

//Матрицы трансформации в instance массив
struct InstanceMatrix {
	glm::mat4 model = glm::mat4(1.f);
	Transform3D transform;
	virtual inline void setPosition(const glm::vec3& position) {
		transform.setPosition(position);
	}
	inline void setRotate(const AngleAxis& angle) {
		transform.setRotate(angle);
	}
	inline void setScale(const glm::vec3& scale) {
		transform.setScale(scale);
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
	virtual void draw(const View* view, const Shader& shader) {
		shader.use();
		view->use(shader);
		object->uniform(shader);
		VAO.draw();
	}
};

class InstanceObject :public GeneralInstance {
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
		VAO.data_draw = DataDraw(VAO.data_draw, models.size());
	}
	virtual void create(VertexBufferObject& buffer, size_t size) {
		VAO.begin();
		VAO.attribInstanceVBOMat4(VAO.sizeAttribute());
		VAO.data_draw = DataDraw(VAO.data_draw, size);
	}
};
#endif
