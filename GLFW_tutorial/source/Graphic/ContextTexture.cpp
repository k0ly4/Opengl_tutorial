#include "ContextTexture.h"
#include "TextureEntity.h"

inline void glTexture::bind2D(const TexPtr& texture) {		bind2D(texture.id()); }
void glTexture::bind2D(const iTexture& texture) {	bind2D(texture.getId()); }
inline void glTexture::bind2D(const TexPtr& texture, size_t text_unit) {
	active(text_unit);
	bind2D(texture.id());
}

inline void glTexture::bind2DArray(const TexPtr& texture) {		bind2DArray(texture.id()); }
void glTexture::bind2DArray(const iTexture& texture) {	bind2DArray(texture.getId()); }

inline void  glTexture::bindCubeMap(const TexPtr& texture) {	bindCubeMap(texture.id());}
void  glTexture::bindCubeMap(const iTexture& texture) {bindCubeMap(texture.getId());}
int glTexture::total_count= 0;
unsigned int glTexture::last = 0;
GLenum glTexture::active_unit = 0;