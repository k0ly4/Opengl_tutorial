#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Resource/Shader.h"
#include "Graphic/Texture.h"
#include "Scene/Camera.h"
#include "Graphic/ContextRender.h"
///Drawable---------------------------------------------
/// <summary>
/// Интерфейс между RenderGeneral* и объектом
/// </summary>
class Drawable {
public:
	inline virtual void draw(const View * view, const Shader & shader) = 0;
	inline glShader::Object getShaderHint()const { return shaderHint; }
protected:
	glShader::Object shaderHint;
};
///---------------------------------------------
/// <summary>
/// Texturable
/// </summary>
class BasisMaterial :public Uniformable {

public:

	BasisMaterial() :
		texture_(0),
		color_(1.f)
	{}

	BasisMaterial(const Texture2D& texture,const Color& color):
		texture_(&texture),
		color_(1.f)
	{}
	BasisMaterial(const Color& color) :
		texture_(0),
		color_(color)
	{}

	void setColor(const Color& color) {
		color_ = color;
	}

	const Color& getColor()const {
		return color_;
	}

	void setTexture(const Texture2D& texture) {
		texture_ = &texture;
	}
	const Texture2D* getTexture()const {return texture_;}

protected:

	const Texture2D* texture_;
	Color color_;

	void uniform(const Shader& shader)const {

		shader.uniform("baseColor", color_);
		if (texture_)
		{
			texture_->use(0);
			shader.uniform("configMaterial", 1);
		}
		else
			shader.uniform("configMaterial", 0);

	}

};

///Texturable---------------------------------------------
/// <summary>
/// Texturable
/// </summary>
class Texturable :public Drawable {
public:

	Texturable() 
	{
		shaderHint = glShader::texture;
	}

	Texturable(glShader::Object shaderHint_) {
		shaderHint = shaderHint_;
	}

	virtual void setColor(const Color& color) {
		material_.setColor(color);
	}
	const Color& getColor()const {
		return material_.getColor();
	}

	virtual void setTexture(const Texture2D& texture) {
		material_.setTexture(texture);
	}

protected:

	BasisMaterial material_;

	inline void uniformMaterial(const Shader& shader)const {
		shader.uniform(material_);
	}

};

///Material---------------------------------------------
class Material:public Uniformable {

public:

	Material():
		baseColor_(1.f),
		specular_(1.f),
		diffuse_(0) 
	{}

	Material(const Color& baseColor, const Texture2D* texture, float specular) :
		baseColor_(baseColor),
		specular_(specular),
		diffuse_(texture) 
	{}

	void setSpecular(float specular) {
		specular_ = specular;
	}

	void setBaseColor(const Color& color) {
		baseColor_ = color;
	}

	const Color& getColor()const {
		return baseColor_;
	}

	void setTexture(const Texture2D& texture) {
		diffuse_ = &texture;
	}

private:

	float specular_;
	Color baseColor_;
	const Texture2D* diffuse_;

	void uniform(const Shader& shader)const {
		shader.uniform("specularMaterial", specular_);
		shader.uniform("baseColor", baseColor_);
		if (diffuse_) {
			diffuse_->use(0);
			shader.uniform("configMaterial", 1);
		}
		else
			shader.uniform("configMaterial", 0);
	}

};

///gbMateriable---------------------------------------------
/// <summary>
///
/// </summary>
class Materiable :public Drawable {

public:

	Materiable()
	{
		shaderHint = glShader::any;
	}

	void setSpecularMaterial(float specular) {
		material.setSpecular(specular);
	}

	void setBaseColor(const Color& color) {
		material.setBaseColor(color);
	}

	void setTexture(const Texture2D& texture) {
		material.setTexture(texture);
	}

protected:

	inline void uniformMaterial(const Shader& shader) const {
		shader.uniform(material);
	}

	Material material;

};
/// <summary>
/// RenderTarget
/// </summary>
class RenderTarget {

public:

	inline void setView(const View& view) { view_ = &view; }
	inline const View* getView()const { return view_; }

	inline void draw(Drawable& object) { object.draw(view_, glShader::get(getHint(object.getShaderHint()))); }

	inline void draw(Drawable& object, glShader::Object index_shader) { object.draw(view_, glShader::get(index_shader)); }

	inline void draw(Drawable& object, const Shader& shader) { object.draw(view_, shader); }

	inline void setDefaultHintShader(glShader::Object hint) { defaultHintShader_ = hint; }

	inline glShader::Object getHintShader(Drawable& object) const { return getHint(object.getShaderHint()); }

protected:

	glShader::Object getHint(glShader::Object objectHint)const {
		if (objectHint == glShader::any)
		{
			return defaultHintShader_;
		}
		else if (objectHint == glShader::any_skeletal_animation)
		{
			return defaultHintShader_ == glShader::gb_texture ? glShader::gb_texture_animation : glShader::any_light_texture;
		}
		else
		{
			return objectHint;
		}
	}

	glShader::Object defaultHintShader_ = glShader::gb_texture;
	const View* view_ = 0;
};

///Graphic---------------------------------------------
/// <summary>
///
/// </summary>
class Graphic {
public:
	Graphic() {
		VAO.data_draw = DataDraw(DataDraw::DrawArrays, sRender::TRIANGLES, 0);
	}
	void setMesh(const std::vector<Vertex>& vertices) {
		VAO.data_draw.data.count_vertex = vertices.size();
		VAO.begin();
		VBO.begin();
		VBO.data(vertices);

		VAO.attrib(0, 3, SIZE_VERTEX, 0);
		VAO.attrib(1, 3, SIZE_VERTEX, 3 * sizeof(float));
		VAO.attrib(2, 2, SIZE_VERTEX, 6 * sizeof(float));

	}

	void draw(const Shader& shader) {
		shader.uniform(material);
		VAO.draw();
	}

	Material material;
	DrawBuffer VAO;
	VertexBufferObject VBO;
};

#endif

