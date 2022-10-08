#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

///Drawable---------------------------------------------
/// <summary>
/// Интерфейс между RenderGeneral* и объектом
/// </summary>
class Drawable {
public:
	virtual void draw(const View * view, const Shader & shader) = 0;
	inline glShader::Object getShaderHint()const {
		return shaderHint;
	}
protected:
	glShader::Object shaderHint;
};

///Texturable---------------------------------------------
/// <summary>
/// Texturable
/// </summary>
class Texturable :public Drawable {
public:

	Texturable() :
		texture_(0),
		color_(1.f) 
	{
		shaderHint = glShader::texture;
	}

	virtual void setColor(const Color& color) {
		color_ = color;
	}
	const Color& getColor(const Color& color)const {
		return color_;
	}

	virtual void setTexture(const Texture2D& texture) {
		texture_ = &texture;
	}

protected:
	
	const Texture2D* texture_ = 0;
	Color color_;

	void uniformMaterial(const Shader& shader)const {

		shader.uniform("baseColor", color_);
		if (texture_)
		{
			texture_->use(0);
			shader.uniform("configMaterial", 1);
		}
		else 
			shader.uniform("configMaterial",0);
		
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
#endif

