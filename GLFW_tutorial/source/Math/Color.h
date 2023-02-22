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
template<typename T>
struct uColor {
    union
    {
        struct { T r, g, b, a; };
        struct { T m[4]; };
    };

    uColor(T r_, T g_, T b_, T a_) :
        r(r_),
        g(g_),
        b(b_),
        a(a_) {}
    uColor(const glm::uvec4& v) :
        r(v.r),
        g(v.g),
        b(v.b),
        a(v.a) {}
    uColor(T r_, T g_, T b_) :
        r(r_),
        g(g_),
        b(b_),
        a(255u){}

    uColor(T rgb) :uColor(rgb,rgb,rgb){}
    uColor() :uColor(0, 0, 0, 0) {}

    inline operator Color()const noexcept       { return Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f); }
    inline operator glm::uvec4()const noexcept  { return  glm::uvec4(r, g, b, a); }


};
template<typename T>
inline glm::ivec4 operator-(const uColor<T>& l, const uColor<T>& r)noexcept { return  glm::ivec4((glm::uvec4)l) - glm::ivec4((glm::uvec4)r); }
template<typename T>
inline glm::uvec4 operator+(const uColor<T>& l, const uColor<T>& r)noexcept { return  (glm::uvec4)l + (glm::uvec4)r; }

//inline glm::uvec4 operator /(const glm::uvec4& l, size_t d) { return { l.x / d,l.y / d,l.z / d,l.w / d };}
inline glm::ivec4 operator /(const glm::ivec4& l, int d) { return { l.x / d,l.y / d,l.z / d,l.w / d }; }
typedef uColor<byte> ColorU;
#endif

