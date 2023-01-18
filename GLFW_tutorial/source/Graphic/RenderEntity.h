#ifndef RENDER_ENTITY_H
#define RENDER_ENTITY_H

#include "Graphic/Drawable.h"

/// <summary>
/// iFrame
/// Èםעונפויס
/// </summary>
class iFrame {

public:

	virtual const glm::ivec2& size()const = 0;
	inline unsigned int id()const  {return id_;}

protected:
	friend class Render;
	unsigned int id_;
};

/// <summary>
/// RenderTarget
/// </summary>
class RenderTarget {

public:

	inline void setView(const View& view) { view_ = &view; }
	inline const View* getView()const {		return view_;}

	inline void draw(Drawable& object) {								object.draw(view_, glShader::get(getHint(object.getShaderHint())));}

	inline void draw(Drawable& object, glShader::Object index_shader) {	object.draw(view_, glShader::get(index_shader));}

	inline void draw(Drawable& object, const Shader& shader) {			object.draw(view_, shader);}

	inline void setDefaultHintShader(glShader::Object hint) {			defaultHintShader_ = hint; }

	inline glShader::Object getHintShader(Drawable& object) const {		return getHint(object.getShaderHint()); }

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
#endif

