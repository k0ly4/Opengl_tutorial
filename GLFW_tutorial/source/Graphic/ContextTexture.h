#ifndef CONTEXT_TEXTURE_H
#define CONTEXT_TEXTURE_H
//#include "Graphic/TextureEntity.h"
#include "Game/Lib/GLFW.h"
#include "Math/GLM.h"
#include "System/Exception.h"
class tWrap2D;
class tWrap3D;
class tFilter;

class iTexture;
class TextureId;
/// <summary>
/// Предоставляет интерфес для загрузки данных
/// </summary>
struct TextureFormatData
{
public:
	
	TextureFormatData(GLint internalformat_, GLint format_) :
		internalFormat(internalformat_),
		format(format_)
	{}
	TextureFormatData():TextureFormatData(GL_RGBA, GL_RGBA){}

	inline void setByChannel(GLint nrChannels,bool gamma) {
		if (nrChannels == 1) {
			internalFormat = format = GL_RED;
		}
		else  if (nrChannels == 3) {
			internalFormat = gamma ? GL_SRGB : GL_RGB;
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
			format = GL_RGBA;
		}
		else LOG(LogError,"Non-known channel of texture\n");
	}
	GLint internalFormat, format;
};


class sTexture {

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

	inline static void getPixels(void *pixels,GLenum target = GL_TEXTURE_2D,GLint level = 0,GLenum format = GL_RGBA,GLenum type = GL_UNSIGNED_BYTE) {
		glGetTexImage(target, level, format, type, pixels);
	}
	inline static void getPixels(byte* pixels, GLenum target = GL_TEXTURE_2D, GLint level = 0, GLenum format = GL_RGBA) {
		glGetTexImage(target, level, format, GL_UNSIGNED_BYTE, pixels);
	}

	static void bind2D(const TextureId& texture);
	static void bind2D(const iTexture& texture);
	static void bind2D(const TextureId& texture, size_t text_unit);

	static void genMipmaps(GLenum target, GLint sizeMips=-1) {
		if(sizeMips > -1) glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, sizeMips);
		glGenerateMipmap(target);
	}
	
	inline static void bind2DArray(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_2D_ARRAY, last);
		}
	}
	 static void bind2DArray(const TextureId& texture);
	 static void bind2DArray(const iTexture& texture);

	 inline static void bindCubeMap(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_CUBE_MAP, last);
		}
	}
	 static void bindCubeMap(const TextureId& texture);
	 static void bindCubeMap(const iTexture& texture);

	  static inline void dataImage3D(GLenum target, const glm::ivec2& size, GLsizei depth, GLint internalFormat, GLint format, const void* data)noexcept {
		  glTexImage3D(target,0,internalFormat,size.x,size.y,depth,0,format,typeByInternalFormat(internalFormat),data);
	  }
	  static inline void dataImage2D(GLenum target, const glm::ivec2& size, GLint internalFormat, GLint format, const void* data)noexcept {
		  glTexImage2D( target,0, internalFormat, size.x, size.y,  0, format, typeByInternalFormat(internalFormat) ,data);
	  }
	  
	  static inline void dataImage2D(const glm::ivec2& size, GLint internalFormat, GLint format, const void* data)noexcept {
		  dataImage2D(GL_TEXTURE_2D, size, internalFormat, format, data);
	  }
	  static inline void dataImage2D(const glm::ivec2& size, const TextureFormatData& format, const void* data)noexcept {
		  dataImage2D(size, format.internalFormat, format.format, data);
	  }
	  static GLenum typeByInternalFormat(GLenum internal_format)noexcept {
		  return internal_format == GL_RGBA16F || internal_format == GL_DEPTH_COMPONENT || internal_format == GL_DEPTH_COMPONENT32F ? GL_FLOAT
			  : GL_UNSIGNED_BYTE;
	  }

	  static void parameter(GLenum target, const tWrap2D& wrap);
	  static void parameter(GLenum target, const tFilter& filter);

	 static int total_count;

private:
	
	static unsigned int last;
	static GLenum		active_unit;

	sTexture() {}
	~sTexture() {}
};
#endif
