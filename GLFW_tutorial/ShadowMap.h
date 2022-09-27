#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include"DepthFrameBuffer.h"

class ShadowMap {
	
public:

	void create(const glm::ivec2& size) {
        map.create(size);
        map.setView(view);
    }

	void orientProjection(const Box& box) {
		view.setProjection(box.getMatrix());
	}

    void orient(const ViewMatrix& _view) {
        view = _view;
    }

	void orientView(const glm::vec3& position,const glm::vec3& direction) {
        view.setView(glm::lookAt(position, position - direction, GAME::WORLD_UP));
	}

    Texture2D getTexture() {
        return map.getTexture();
    }

    void uniform(const Shader& shader, size_t unit, const std::string& name);

    void render(void (*render_scene) (RenderTarget& target));

    void render(RenderClass* render);;

private:

    RenderTextureDepth map;
    ViewMatrix view;

};

#endif
