#ifndef CONTEXT_TEXTURE_H
#define CONTEXT_TEXTURE_H
#include "ResourceTypes.h"

class glTexture {

public:
	static void active(size_t text_unit);
	static void bind2D(unsigned int texture);
	static void bind2D(GeneralTexture& texture);
	static void bind2D(GeneralTexture& texture, size_t text_unit);
	static void bind2DArray(unsigned int texture);
	static void bindCubeMap(unsigned int texture);
	static void bindCubeMap(GeneralTexture& texture);
private:
	static unsigned int last;
	static GLenum active_unit;
	glTexture() {}
	~glTexture() {}
};
#endif
