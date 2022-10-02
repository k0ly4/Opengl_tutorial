#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include "Light.h"
#include "Camera.h"

class PointLight:public Light {

public:

    PointLight(const glm::vec3& color,
        const glm::vec3& position,
        const glm::vec2& attenuation)
        : attenuation_(attenuation), 
        position_(position),
        Light(color),
        need_up_radius(1)
    {
        shadow_map_ = std::make_shared<RenderDepthCubeMap>(glm::ivec2(512), getRadius());
    }

    PointLight() :
        position_(0.f),
        attenuation_(0.f),
        need_up_radius(0),
        radius_(0.f)
    { 
        shadow_map_ = std::make_shared<RenderDepthCubeMap>(glm::ivec2(512),getRadius());
    }
    void setColor(const glm::vec3& color) {
        if (color == color_) return;
        color_ = color;
        need_up_radius = 1;
        shadow_map_->setFarPlane(getRadius());
    }

    void setPosition(const glm::vec3& position) {
        position_ = position;
    }

    const glm::vec3& getPosition()const {
        return position_;
    }

    void setAttenuation(const glm::vec2& attenuation) {
        attenuation_ = attenuation; 
        need_up_radius = 1;
        shadow_map_->setFarPlane(getRadius());
    }
    const glm::vec2& getAttenuation()const {
        return attenuation_;
    }

    void uniform(const std::string& name, const Shader& shader)const {
        Light::uniform(name, shader);
        shader.uniform(name + ".position", position_);
        shader.uniform(name + ".attenuation", attenuation_);
    }

    //shadow
    inline void uniformShadow(const std::string& name, const Shader& shader, size_t unit) {
        shadow_map_->uniform(shader, name, unit);
    }

    inline void createMap(const glm::ivec2& size) {
        shadow_map_->create(size,getRadius());
    }

    inline void upShadowMap(RenderScene& render_class, const Camera& camera) {
        shadow_map_->render(camera,position_, render_class);
    }

    inline float getRadius()const {
        if (need_up_radius) calcRadius();
        return radius_;
    }

private:
    mutable bool need_up_radius;
    mutable float radius_;
   
    std::shared_ptr<RenderDepthCubeMap> shadow_map_;
    glm::vec3 position_;
    glm::vec2 attenuation_;

    void calcRadius()const {
        need_up_radius = 0;
        constexpr float dark = 4.f/ 256.f;
        radius_ = getLimitPropagation(std::max(std::max(color_.x, color_.y), color_.z), dark, glm::vec3(attenuation_, 1.f));
;    }
};
#endif