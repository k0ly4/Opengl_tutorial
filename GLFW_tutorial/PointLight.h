#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include "Light.h"
#include "Camera.h"

class PointLight:public Light {
public:

    PointLight(const glm::vec3& color,
        const glm::vec3& position,
        const glm::vec2& attenuation)
        : attenuation_(attenuation), position_(position),Light(color)
    {}

    PointLight():
        position_(0.f),
        attenuation_(1.f) 
    {}

    void setPosition(const glm::vec3& position) {
        position_ = position;
    }

    const glm::vec3& getPosition()const {
        return position_;
    }

    void setAttenuation(const glm::vec2& attenuation) {
        attenuation_ = attenuation;
    }
    const glm::vec2& getAttenuation()const {
        return attenuation_;
    }

    void uniform(const std::string& name, const Shader& shader)const {
        Light::uniform(name, shader);
        shader.uniform(name + ".position", position_);
        shader.uniform(name + ".attenuation", attenuation_);
    }

private:

    glm::vec3 position_;
    glm::vec2 attenuation_;

};
#endif