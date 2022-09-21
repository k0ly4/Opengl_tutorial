#include "ResourceTypes.h"


 void glTexture::active(size_t text_unit) {
		if (text_unit != active_unit) {
			active_unit = text_unit;
			glActiveTexture(active_unit);
		}
	}
 void glTexture::bind2D(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_2D, last);
		}
	}
 void glTexture::bind2D(GeneralTexture& texture) {
		bind2D(texture.getID());
	}
 void glTexture::bind2D(GeneralTexture& texture, size_t text_unit) {
		active(text_unit);
		bind2D(texture.getID());
	}
 void glTexture::bind2DArray(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_2D_ARRAY, last);
		}
	}
 void glTexture::bindCubeMap(unsigned int texture) {
		if (texture != last) {
			last = texture;
			glBindTexture(GL_TEXTURE_CUBE_MAP, last);
		}
	}
 void glTexture::bindCubeMap(GeneralTexture& texture) {
		bindCubeMap(texture.getID());
	}
unsigned int glTexture::last = 0;
GLenum glTexture::active_unit = 0;


	GLenum GlImage::format() {
		if (nrChannels == 3)
			return GL_RGB;
		else if (nrChannels == 4)
			return  GL_RGBA;
		else return GL_RED;
	}
	GLenum GlImage::internal_format(bool gamma) {
		if (nrChannels == 3)
			return gamma ? GL_SRGB : GL_RGB;
		else if (nrChannels == 4)
			return  gamma ? GL_SRGB_ALPHA : GL_RGBA;
		else return GL_RED;
	}
