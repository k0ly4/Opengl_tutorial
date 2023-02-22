#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H
#include "Graphic/Texture.h"
#include "System/Arrays.h"

class TextureMap {
public:

	TextureMap() {}

	void create(glm::ivec2 size) {map.setSize(size);}

	void setWrap(const tWrap2D& wrap_)		{tex.resource->setWrap(wrap_);}

	void setFilter(const tFilter& filter_)	{tex.resource->setFilter(filter_);}

	inline void upTexture() {
		tex.create(map.v_size(), { GL_RGBA,GL_RGBA }, (void*)map.data());
	}

	void testClear() {
		for (size_t i = 0; i < map.size(); i++) {
			map[i] = ColorU((i/2) % 255);
		}
		modified = 1;
	}
	inline const Texture2D& texture()noexcept {
		if (modified) {
			upTexture();
			modified = 0;
			
		}
		return tex;
	}
	inline int exportToPNG(const char* path)noexcept {return ImageLoader::savePNG(path, map.v_size(), 4, map.data());}

	vector2d<ColorU> map;
	bool modified = 1;

private:

	Texture2D tex;
};

#endif

