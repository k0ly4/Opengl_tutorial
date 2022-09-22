#ifndef RENDER_ENTITY_H
#define RENDER_ENTITY_H

#include "Type_Object.h"

class GeneralRender {

public:
	const glm::ivec2& getSize() {
		return size;
	}

protected:
	glm::ivec2 size;
	friend class GlRender;
	unsigned int id;
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
