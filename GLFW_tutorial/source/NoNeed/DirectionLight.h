#ifndef DIRECTION_LIGHT_H
#define DIRECTION_LIGHT_H

#include "Light.h"
#include "Camera.h"
#include "CascadedShadow.h"
#include "ShadowMap.h"

////SimpleDirLight------------------------------------
/// <summary>
/// 
/// </summary>
class SimpleDirLight :public Light {
public:

    SimpleDirLight()
        :direction_(0.f),
        Light() {}

    SimpleDirLight(
        const glm::vec3& color,
        const glm::vec3& direction
    ) :
        Light(color),
        direction_(glm::normalize(direction)) {}

    inline void upShadowMap(RenderScene& render_class, const Camera& camera) {
        map_.render(camera, render_class);
    }

    inline void uniformShadow(const std::string& name, const Shader& shader, size_t unit) {
        map_.uniform(shader, name, unit);
    }

    inline void setDirection(const View3D& camera) {
        direction_ = -glm::normalize(camera.getBasis().front);
        map_.setView(camera);
    }

    void uniform(const std::string& name, const Shader& shader) {
        Light::uniform(name, shader);
        shader.uniform(name + ".direction", direction_);
    }

    ShadowMap map_;
private:

    glm::vec3 direction_;

};
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

    inline void setSizeMap(const glm::ivec2& size) {
        shadow_maps_.setSize(size);
    }

    void setMultShadow(float value) {
        shadow_maps_.setMult(value);
    }

    const ArrayTexture2D& getShadowMap()const {
        return shadow_maps_.getTexture();
    }

    inline void upShadowMap(RenderScene& render_class, const Camera& camera) { 
        shadow_maps_.render(camera, direction_, render_class);
    }

    inline void uniformShadow(const std::string& name, const Shader& shader, size_t unit) {
        shadow_maps_.uniform(shader, name, unit);
    }

    inline void setDirection(const glm::vec3& direction) {
        direction_ = glm::normalize(direction);
    }

    void uniform(const std::string& name, const Shader& shader) {
        Light::uniform(name, shader);
        shader.uniform(name + ".direction", direction_);
    }

    void uniform(const std::string& name, const Shader& shader,size_t index_cascade) {
        Light::uniform(name, shader);
        shader.uniform(name + ".direction", direction_);     
        shadow_maps_.uniform(shader, 3, index_cascade, name);
    }

private:

    CascadeShadow shadow_maps_;
    glm::vec3 direction_;

};
#endif