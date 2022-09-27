#ifndef RENDER_ENTITY_H
#define RENDER_ENTITY_H

#include "TypeObject.h"

class GeneralRender {

public:
	const glm::ivec2& getSize()const {
		return size_;
	}

protected:
	glm::ivec2 size_;
	friend class GlRender;
	unsigned int id_;
};

class RenderTarget {

public:
	inline void setView(View& view) {
		this->view = &view;
	}
	inline void draw(Drawable& object) {
		object.draw(view, glShader::get(object.id_obj));
	}
	inline void draw(Drawable& object, glShader::Object index_shader) {
		object.draw(view, glShader::get(index_shader));
	}
	inline void draw(Drawable& object, const Shader& shader) {
		object.draw(view, shader);
	}

protected:
	View* view = 0;

};
#endif

