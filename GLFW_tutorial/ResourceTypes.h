
#ifndef RESOURCE_TYPES_H
#define RESOURCE_TYPES_H
#include"stb_image.h"
#include "Math.h"
/// <summary>
/// Resource
/// </summary>
class Resource {
public:
};

/// <summary>
/// TextureID
/// </summary>
class TextureID {
public:
	unsigned int id;

	TextureID() {
		glGenTextures(1, &id);
	}

	~TextureID() {
		glDeleteTextures(1, &id);
	}
};

/// <summary>
/// TextureShell
/// </summary>
class TexturePointer {

public:

	TexturePointer():
		texture(std::make_shared<TextureID>())
	{}

	inline bool reBuild() {
		if (texture.unique() == 0) {
			texture = std::make_shared<TextureID>();
			return 1;
		}
		return 0;
	}

	inline unsigned int get() const {
		return texture->id;
	}

private:
	std::shared_ptr<TextureID> texture;
};

/// <summary>
/// TextureDataFormat
/// </summary>
struct TextureDataFormat
{
	GLint
		internal_format,
		format,
		filter;

	TextureDataFormat(GLint  text_internalformat, GLint  text_format, GLint text_filter) :
		internal_format(text_internalformat), 
		format(text_format), 
		filter(text_filter) 
	{}

	void setDataTexture2D(const glm::ivec2& size,const void * data) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			internal_format,
			size.x,
			size.y,
			0,
			format,
			getType(internal_format),
			data);
	}

	void setParameteriTexture2D() {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	static GLenum getType(GLenum internal_format) {
		return 	internal_format == GL_RGBA16F || internal_format == GL_DEPTH_COMPONENT || internal_format == GL_DEPTH_COMPONENT32F ? 
			GL_FLOAT : GL_UNSIGNED_BYTE;
	}

};

/// <summary>
/// Texture
/// </summary>
class Texture {

public:

	Texture(const TexturePointer& id) :id_(id) {}
	Texture() {}

	const TexturePointer& getId()const {
		return id_;
	}

	virtual inline bool detach() {
		return id_.reBuild();
	}
protected:
	TexturePointer id_;
};
#endif