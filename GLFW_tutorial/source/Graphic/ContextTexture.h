#ifndef CONTEXT_TEXTURE_H
#define CONTEXT_TEXTURE_H
//#include "Graphic/TextureEntity.h"
#include "Game/Lib/GLFW.h"
class TexPtr;
class iTexture;

class glTexture {

public:
	
	inline static void gen(unsigned int& id) {
		glGenTextures(1, &id);
		total_count++;
	}
	inline static void free(unsigned int id) {	
		glDeleteTextures(1, &id);
		total_count--;
	}

	inline static void active(size_t text_unit) {
		if (text_unit != active_unit) {
			active_unit = text_unit;
			glActiveTexture(active_unit);
		}
	}

	inline static void bind2D(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_2D, last);
		}
	}
	static void bind2D(const TexPtr& texture);
	static void bind2D(const iTexture& texture);
	static void bind2D(const TexPtr& texture, size_t text_unit);

	inline static void bind2DArray(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_2D_ARRAY, last);
		}
	}
	 static void bind2DArray(const TexPtr& texture);
	 static void bind2DArray(const iTexture& texture);

	 inline static void bindCubeMap(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_CUBE_MAP, last);
		}
	}
	 static void bindCubeMap(const TexPtr& texture);
	 static void bindCubeMap(const iTexture& texture);

	 static int total_count;
private:
	
	static unsigned int last;
	static GLenum		active_unit;

	glTexture() {}
	~glTexture() {}
};
#endif
