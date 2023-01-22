#ifndef COLOR_HPP
#define COLOR_HPP

#include"Game/Lib/GLFW.h"
#include "Math/GLM.h"

struct Color
{
    static const Color 
        RED, BLUE, WHITE, BLACK, GREEN, YELLOW, PURPLE, CYAN;

    union 
    {
        struct { float m[4]; };
        struct { float r, g, b, a; };
        struct { glm::vec4 vec4; };
        struct { glm::vec3 vec3; };
    };
    
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
};

inline bool operator ==(const Color& left, const Color& right) {
    return ((left.r == right.r) &&
        (left.b == right.b) &&
        (left.g == right.g) &&
        (left.a == right.a));
}
inline bool operator !=(const Color& left, const Color& right) {
    
        return !(left == right);
    
}

struct ColorU {
    union
    {
        struct { byte r, g, b, a; };
        struct { byte m[4]; };
    };

    ColorU(byte r_, byte g_, byte b_, byte a_) :
        r(r_),
        g(g_),
        b(b_),
        a(a_) {}
    ColorU(byte r_, byte g_, byte b_) :
        r(r_),
        g(g_),
        b(b_),
        a(255u){}
    ColorU(byte rgb) :ColorU(rgb,rgb,rgb){}
    ColorU() :ColorU(0, 0, 0, 0) {}
    operator Color() { return Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f); }
   
};
#endif

