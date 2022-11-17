#ifndef COLOR_HPP
#define COLOR_HPP

#include"Game/Lib/GLFW.h"
#include "Math/GLM.h"

struct Color
{
    static const Color
        RED, BLUE, WHITE, BLACK, GREEN, YELLOW, PURPLE, CYAN;

    float r, g, b, a;
    Color();
    Color(const glm::vec3& color);
    Color(const glm::vec4& color);
    Color(float red, float green, float blue);
    Color(float red, float green, float blue, float alpha);
    Color(float factor);

    static Color ColorU(const glm::uvec4& color);
    static Color ColorU(byte red, byte green, byte blue, byte alpha);
    static Color ColorU(byte red, byte green, byte blue);
    static Color ColorU(const glm::uvec3& color);

    glm::vec4 vec4()const;
    glm::vec3 vec3()const;
};
bool operator ==(const Color& left, const Color& right);
bool operator !=(const Color& left, const Color& right);
#endif

