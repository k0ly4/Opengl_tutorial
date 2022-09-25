#ifndef DIRECTION_LIGHT_H
#define DIRECTION_LIGHT_H
#include "RenderWindow.h"
#include "Camera.h"
#include "CascadedShadow.h"
#include "ShadowMap.h"

class D_Light_Shadow {
    ShadowMap shadow_map;
    glm::vec3 direction;
public:
    D_Light_Shadow() {
        float factor = 30.f;
        shadow_map.orientProjection(Box(-factor, factor, -factor, factor, 1.f, 100.f));
    }
    void setOrient(const glm::vec3& Direction, const glm::vec3& position) {
        direction = glm::normalize(Direction);
        shadow_map.orientView(position, direction);
    }
};

class D_Light {
    glm::vec3 direction;
    friend class LightSystem;
    CascadeShadow cascaded_shadow_maps;
public:
    glm::vec3 color;
    D_Light() {}
    D_Light(
        const glm::vec3& Color,
        const glm::vec3& Direction,
        const glm::vec3& Position
    ) :color(Color), direction(glm::normalize(Direction))
    {}
    void createMap(int width, int height,View& camera) {
        cascaded_shadow_maps.create(camera, direction);
    }
    void setDirection(const glm::vec3& _irection) {
        direction = glm::normalize(_irection);
        cascaded_shadow_maps.setDirection(direction);
    }
    void render(RenderClass* render_class, Camera& camera) {
        cascaded_shadow_maps.render(camera,render_class);
    }
    g_ArrayTexture2D getMap() {
        return cascaded_shadow_maps.getTexture();
    }
    void uniform(const std::string& name, const Shader& shader) {
        shader.uniform(name + ".direction", direction);
        shader.uniform(name + ".color", color);
        cascaded_shadow_maps.uniform(shader, name, 3);
       
    }
    void uniform(const std::string& name, const Shader& shader,size_t index_cascade) {
        shader.uniform(name + ".direction", direction);
        shader.uniform(name + ".color", color);
        cascaded_shadow_maps.uniform(shader, 3, index_cascade, name);
    }
};
#endif