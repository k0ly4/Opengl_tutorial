#ifndef CONTEXT_TEXTURE_H
#define CONTEXT_TEXTURE_H
#include "ResourceTypes.h"

class glTexture {

public:

	static void active(size_t text_unit);

	static void bind2D(unsigned int texture);
	inline static void bind2D(const TexturePointer& texture) {
		bind2D(texture.get());
	}
	inline static void bind2D(const Texture& texture) {
		bind2D(texture.getId());
	}
	static void bind2D(const TexturePointer& texture, size_t text_unit);

	static void bind2DArray(unsigned int texture);
	inline static void bind2DArray(const TexturePointer& texture)
	{
		bind2DArray(texture.get());
	}
	inline static void bind2DArray(const Texture& texture)
	{
		bind2DArray(texture.getId());
	}

	static void bindCubeMap(unsigned int texture);
	inline static void bindCubeMap(const TexturePointer& texture) {
		bindCubeMap(texture.get());
	}
	inline static void bindCubeMap(const Texture& texture) {
		bindCubeMap(texture.getId());
	}
private:

	static unsigned int last;
	static GLenum active_unit;

	glTexture() {}
	~glTexture() {}
};
#endif
