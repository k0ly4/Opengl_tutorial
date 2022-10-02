#ifndef RENDER_ENTITY_H
#define RENDER_ENTITY_H

#include "TypeObject.h"

/// <summary>
/// GeneralRender
/// </summary>
class GeneralRender {

public:

	virtual const glm::ivec2& getSize()const = 0;

	unsigned int getId()const  {
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

	inline void draw(Drawable& object) {
		object.draw(view_, glShader::get(object.id_obj));
	}

	inline void draw(Drawable& object, glShader::Object index_shader) {
		object.draw(view_, glShader::get(index_shader));
	}

	inline void draw(Drawable& object, const Shader& shader) {
		object.draw(view_, shader);
	}

protected:

	const View* view_ = 0;
};
#endif

