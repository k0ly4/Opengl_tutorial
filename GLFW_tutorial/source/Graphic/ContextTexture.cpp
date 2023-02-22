#include "ContextTexture.h"
#include "TextureEntity.h"

inline void sTexture::bind2D(const TextureId& texture) {		bind2D(texture.get()); }
void sTexture::bind2D(const iTexture& texture) {				bind2D(texture.getId()); }
void sTexture::bind2D(const TextureId& texture, size_t text_unit) {
																active(text_unit);
																bind2D(texture.get());
}

inline void sTexture::bind2DArray(const TextureId& texture) {		bind2DArray(texture.get()); }
void sTexture::bind2DArray(const iTexture& texture) {				bind2DArray(texture.getId()); }

inline void  sTexture::bindCubeMap(const TextureId& texture) {		bindCubeMap(texture.get());}
void  sTexture::bindCubeMap(const iTexture& texture) {				bindCubeMap(texture.getId());}



void sTexture::parameter(GLenum target, const tWrap2D& wrap) {
	wrap.setup(target);
}
void sTexture::parameter(GLenum target, const tFilter& filter) {
	filter.setup(target);
}

int sTexture::total_count= 0;
unsigned int sTexture::last = 0;
GLenum sTexture::active_unit = 0;