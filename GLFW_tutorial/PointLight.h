#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include "RenderWindow.h"
#include "Camera.h"
class P_Light {
public:
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 attenuation;
    P_Light(const glm::vec3& Color,
        const glm::vec3& Position,
        const glm::vec2& Attenuation)
        :color(Color), attenuation(Attenuation), position(Position)
    {}
    P_Light() {}
    void uniform(const std::string& name, const Shader& shader) {
        shader.uniform(name + ".position", position);
        shader.uniform(name + ".color", color);
        shader.uniform(name + ".attenuation", attenuation);
    }
};
#endif