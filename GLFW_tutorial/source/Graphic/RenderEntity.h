#ifndef RENDER_ENTITY_H
#define RENDER_ENTITY_H

#include "Scene/TypeObject.h"

/// <summary>
/// GeneralRender
/// </summary>
class GeneralRender {

public:

	virtual const glm::ivec2& getSize()const = 0;

	inline unsigned int getId()const  {
		return id_;
	}

protected:

	unsigned int id_;
};

/// <summary>
/// RenderTarget
/// </summary>
class RenderTarget {

public:

	inline void setView(const View& view) {
		view_ = &view;
	}
	inline const View* getView()const {
		return view_;
	}

	inline void draw(Drawable& object) {
		object.draw(view_, glShader::get(getHint(object.getShaderHint())));
	}

	inline void draw(Drawable& object, glShader::Object index_shader) {
		object.draw(view_, glShader::get(index_shader));
	}

	inline void draw(Drawable& object, const Shader& shader) {
		object.draw(view_, shader);
	}
	void setDefaultHintShader(glShader::Object hint) {
		defaultHintShader_ = hint;
	}
	glShader::Object getHintShader(Drawable& object) {
		return getHint(object.getShaderHint());
	}
protected:

	glShader::Object getHint(glShader::Object objectHint) {
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
#endif

