
#ifndef RESOURCE_TYPES_H
#define RESOURCE_TYPES_H
#include"stb_image.h"
#include "Math.h"

class GeneralTexture {

public:
	void gen() {
		texture = std::make_unique<g_texture>();
	}
	inline unsigned int getID() const {
		return texture.get()->id;
	}

protected:
	friend class glTexture;
	struct g_texture {
		unsigned int id;
		g_texture() {
			glGenTextures(1, &id);
		}
		~g_texture() {
			glDeleteTextures(1, &id);
		}
	};
	friend class Texture2D;
	friend class g_Texture2D;
	friend class g_ArrayTexture2D;
	std::shared_ptr<g_texture> texture;

private:

};
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

class GlImage :public GLFWimage {

public:
	int nrChannels;
	GLenum format();
	GLenum internal_format(bool gamma);
	~GlImage() {
		stbi_image_free(pixels);
	}
private:
};
#endif