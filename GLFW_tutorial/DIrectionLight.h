#ifndef DIRECTION_LIGHT_H
#define DIRECTION_LIGHT_H
#include "RenderWindow.h"
#include "Camera.h"
#include "CascadedShadow.h"
#include "ShadowMap.h"
class D_Light {
    glm::vec3 direction;
    friend class LightSystem;
    CascadeShadow cascaded_shadow_maps;
    ShadowMap shadow_map;
public:
    glm::vec3 color;
    D_Light() {}
    D_Light(
        const glm::vec3& Color,
        const glm::vec3& Direction,
        const glm::vec3& Position
    ) :color(Color), direction(glm::normalize(Direction))
    {
  
        float factor = 30.f;
      //  view.setProjection(FloatRect(-factor, factor, factor, -factor), 1.f, 100.f);
        shadow_map.orientProjection(Box(-factor, factor, -factor, factor, 1.f, 100.f));
    }
    void createMap(int width, int height,View& camera) {
        shadow_map.create(glm::ivec2(width, height));
        cascaded_shadow_maps.create(camera, direction);
    }
  /*  void setOrient(const glm::vec3& Direction,const glm::vec3& position) {
        direction = glm::normalize(Direction);
        shadow_map.orientView(position, direction);
        cascaded_shadow_maps.setDirection(direction);
    }*/
    void setDirection(const glm::vec3& _irection) {
        direction = glm::normalize(_irection);
    }
    void setOrient(Camera& camera) {
       // direction = glm::normalize(-camera.Transform().Basis().front);
       // shadow_map.orientView(camera.getPosition(), direction);
        cascaded_shadow_maps.setDirection(direction);
        shadow_map.orient(cascaded_shadow_maps.getView(camera, 0));   
    }
    //void uniform(const std::string& name, const Shader& shader,View& view) {
    //    shader.uniform(name + ".direction", direction);
    //    shader.uniform(name + ".color", color);

    //   // shader.uniform(name + ".gWVP", view.getVP());
    //    //cascaded_shadow_maps.bind(3,shader,name);
    //}
    //void uniform(const std::string& name, const Shader& shader,size_t index) {
    //    shader.uniform(name + ".direction", direction);
    //    shader.uniform(name + ".color", color);
    //    cascaded_shadow_maps.bind(3, index, shader, name);
    //}
    void render(RenderClass* render_class) {
        shadow_map.render(render_class);
    }
    void render(void (*render_scene) (RenderTarget& target)) {
        shadow_map.render(render_scene);
    }
    Texture2D getMap(size_t index) {
        return shadow_map.getTexture();
    }
    void uniform(const std::string& name, const Shader& shader) {
        shader.uniform(name + ".direction", direction);
        shader.uniform(name + ".color", color);
        shadow_map.uniform(shader, 3, name);
    }
};
#endif