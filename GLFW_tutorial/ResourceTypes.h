
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
struct TextureDataFormat
{
	GLint
		internal_format,
		format,
		filter;

	TextureDataFormat(GLint  text_internalformat, GLint  text_format, GLint text_filter) :
		internal_format(text_internalformat), format(text_format), filter(text_filter) {}
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