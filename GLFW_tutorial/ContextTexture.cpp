#include "ContextTexture.h"

void glTexture::active(size_t text_unit) {
	if (text_unit != active_unit) {
		active_unit = text_unit;
		glActiveTexture(active_unit);
	}
}
/// <summary>
/// Texture2D
/// </summary>
void glTexture::bind2D(unsigned int texture) {
	if (texture != last) {
		last = texture;
		glBindTexture(GL_TEXTURE_2D, last);
	}
}

void glTexture::bind2D(const TexturePointer& texture, size_t text_unit) {
	active(text_unit);
	bind2D(texture.get());
}


/// <summary>
/// TextureArray
/// </summary>
void glTexture::bind2DArray(unsigned int texture) {
	if (texture != last) {
		last = texture;
		glBindTexture(GL_TEXTURE_2D_ARRAY, last);
	}
}

/// <summary>
/// TextureCube
/// </summary>
void glTexture::bindCubeMap(unsigned int texture) {
	if (texture != last) {
		last = texture;
		glBindTexture(GL_TEXTURE_CUBE_MAP, last);
	}
}
unsigned int glTexture::last = 0;
GLenum glTexture::active_unit = 0;