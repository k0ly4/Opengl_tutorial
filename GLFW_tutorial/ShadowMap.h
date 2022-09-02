#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H
#include"ShadowFrameBuffer.h"
class ShadowMap {
	RenderTextureDepth map;
	View3D view;
public:
	void create(const glm::ivec2& size) {
        map.create(size);
        map.setView(view);
    }
	void orientProjection(const Box& box) {
		view.setProjection(box);
	}
    void orient(const View& _view) {
        view.orient(_view);
    }
	void orientView(const glm::vec3& position,const glm::vec3& direction) {
		view.setPosition(position);
		view.Transform().look(direction);
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
                // ѕрив€зываем и очищаем текущий каскад
            GlRender::bind(map);
           glClear(GL_DEPTH_BUFFER_BIT);
           render_scene(map);
        }
        void render(RenderClass* render)
        {
            GlRender::bind(map);
            glClear(GL_DEPTH_BUFFER_BIT); 
            render->drawScene_shadow(map);    
        }
};

#endif
