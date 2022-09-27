#ifndef DIRECTION_LIGHT_H
#define DIRECTION_LIGHT_H

#include "Light.h"
#include "Camera.h"
#include "CascadedShadow.h"
#include "ShadowMap.h"

class DirectionLight :public Light {
   
public:

    DirectionLight()
        :direction_(0.f),
        Light() {}

    DirectionLight(
        const glm::vec3& color,
        const glm::vec3& direction
    ) : Light(color),
        direction_(glm::normalize(direction))
    {}

    void createMap(int width, int height,View& camera) {
        shadow_maps_.setDirectionLight(direction_);
    }

    void setDirection(const glm::vec3& direction) {
        direction_ = glm::normalize(direction);
        shadow_maps_.setDirectionLight(direction_);
    }

    void render(RenderScene& render_class, Camera& camera) {
        shadow_maps_.render(camera,render_class);
    }

    ArrayTexture2D getMap() {
        return shadow_maps_.getTexture();
    }

    void uniform(const std::string& name, const Shader& shader) {
        Light::uniform(name, shader);
        shader.uniform(name + ".direction", direction_);
        shadow_maps_.uniform(shader, name, 3);
       
    }

    void uniform(const std::string& name, const Shader& shader,size_t index_cascade) {
        Light::uniform(name, shader);
        shader.uniform(name + ".direction", direction_);     
        shadow_maps_.uniform(shader, 3, index_cascade, name);
    }

private:
    CascadeShadow shadow_maps_;
    glm::vec3 direction_;

    friend class LightSystem;
};
#endif