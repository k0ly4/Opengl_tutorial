#include "ResourceTypes.h"

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
