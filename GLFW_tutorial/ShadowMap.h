#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H
#include"ShadowFrameBuffer.h"
class ShadowMap {
	RenderTextureDepth map;
	ViewMatrix view;
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
		void uniform(const Shader& shader,size_t unit,const std::string& name) {
			map.getTexture().use(unit);
			shader.uniform(name + ".spaceMatrix", view.getVP());
		}
        void render(void (*render_scene) (RenderTarget& target))
        {         
                // ����������� � ������� ������� ������
            GlRender::bind(map);
           glClear(GL_DEPTH_BUFFER_BIT);
           render_scene(map);
        }
        void render(RenderClass* render)
        {
            GlRender::bind(map);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            render->drawScene_shadow(map, glShader::shadow_depth);
        }
};

#endif
