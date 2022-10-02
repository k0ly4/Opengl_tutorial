#ifndef DIRECTION_LIGHT_H
#define DIRECTION_LIGHT_H

#include "Light.h"
#include "Camera.h"
#include "CascadedShadow.h"
#include "ShadowMap.h"

/// <summary>
/// DirectionLight
/// </summary>
class DirectionLight :public Light {

public:

    DirectionLight()
        :direction_(0.f),
        Light() {}

    DirectionLight(
        const glm::vec3& color,
        const glm::vec3& direction
    ) :
        Light(color),
        direction_(glm::normalize(direction)) {}

    void setActive(bool enable) {
        active = enable;
    }
    inline void setSizeMap(const glm::ivec2& size) {
        shadow_maps_.setSize(size);
    }

    void setMultShadow(float value) {
        shadow_maps_.setMult(value);
    }

    const ArrayTexture2D& getShadowMap()const {
        return shadow_maps_.getTexture();
    }

    void upShadowMap(RenderScene& render_class, const Camera& camera) {
        if (active == 0)
            return;
        shadow_maps_.render(camera, direction_, render_class);
    }

    void uniformShadow(const std::string& name, const Shader& shader, size_t unit) {
        if (active == 0)
            return;
        shadow_maps_.uniform(shader, name, unit);
    }

    inline void setDirection(const glm::vec3& direction) {
        direction_ = glm::normalize(direction);
    }

    void uniform(const std::string& name, const Shader& shader) {
        if (active == 0) {
            shader.uniform(name + ".enable", active);
            return;
        }
            
        Light::uniform(name, shader);
        shader.uniform(name + ".direction", direction_);
    }

    void uniform(const std::string& name, const Shader& shader,size_t index_cascade) {
        if (active == 0) {
            shader.uniform(name + ".enable", active);
            return;
        }
        Light::uniform(name, shader);
        shader.uniform(name + ".direction", direction_);     
        shadow_maps_.uniform(shader, 3, index_cascade, name);
    }

private:

    bool active = 0;
    CascadeShadow shadow_maps_;
    glm::vec3 direction_;

};
#endif