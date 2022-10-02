#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
/// <summary>
/// Drawable
/// Интерфейс между RenderGeneral* и объектом
/// </summary>
class Drawable {
public:
	glShader::Object id_obj;
	virtual void draw(const View * view, const Shader & shader) = 0;
};

/// <summary>
/// gbMateriable
/// </summary>
class gbMateriable :public Drawable {

public:
	void setSpecular(float _specular) {
		specular = _specular;
	}
	void setColor(const glm::vec3& _color) {
		base_color = _color;
		diffuse = 0;
		id_obj = glShader::gb_color_uniform;
	}
	void setColor(const Color& color) {
		setColor(color.vec3());
	}
	void setTexture(Texture2D& texture) {
		id_obj = glShader::gb_texturable;
		this->diffuse = &texture;
	}

protected:

	float specular = 0.1f;
	glm::vec3 base_color = glm::vec3(1.f);
	Texture2D* diffuse = 0;

	void uniformMaterial(const Shader& shader) {
		shader.uniform("specularMaterial", specular);
		if (diffuse) diffuse->use(0);
		else shader.uniform("color", base_color);
	}
};
#endif

