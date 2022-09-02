#ifndef ABSTRACTION_H
#define ABSTRACTION_H

#include"LibInit.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <map>
#define PI 3.1415926535f
typedef unsigned char byte;
class GAME{
    GAME() {}
    ~GAME() {}
public:
       static const glm::vec3 WORLD_UP;
       static const glm::ivec2 RENDER_SIZE;
};
const glm::vec3 GAME::WORLD_UP = glm::vec3(0.f, 1.f, 0.f);
const glm::ivec2 GAME::RENDER_SIZE = glm::ivec2(800, 600);

struct Color
{
    static const Color
        RED,BLUE,WHITE,BLACK,GREEN,YELLOW,PURPLE,CYAN;

    float r, g, b, a;
    Color() :r(0), g(0), b(0), a(1.f) {}
    Color(const glm::vec3& color) :r(color.x), g(color.g), b(color.b), a(1.f) {}
    Color(const glm::vec4& color) :r(color.x), g(color.g), b(color.b), a(color.w) {}
    Color(float red, float green, float blue) :r(red), g(green), b(blue), a(1.f) {}
    Color(float red, float green, float blue, float alpha) :r(red), g(green), b(blue), a(alpha) {}
    Color(float factor) :r(factor), g(factor), b(factor), a(1.f) {}
    
    static Color ColorU(const glm::uvec4&color) {
        return Color(color.r / 255.f, color.g/255.f, color.b / 255.f, color.a / 255.f);
    }
    static Color ColorU(byte red, byte green, byte blue, byte alpha) {
        return Color(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
    }
    static Color ColorU(byte red, byte green, byte blue) {
        return Color(red / 255.f, green / 255.f, blue / 255.f, 1.f);
    }
    static Color ColorU(const glm::uvec3& color) {
        return Color(color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.f);
    }
 
    glm::vec4 vec4()const {
        return glm::vec4(r, g, b, a);
    }
    glm::vec3 vec3()const {
        return glm::vec3(r, g, b);
    }
};
const Color Color::RED (1.f, 0.f, 0.f);
const Color Color::BLUE(0.f, 0.f, 1.f);
const Color Color::WHITE(1.f, 1.f, 1.f);
const Color Color::BLACK(0.f, 0.f, 0.f);
const Color Color::GREEN(0.f, 1.f, 0.f);
const Color Color::YELLOW(1.f, 1.f, 0.f);
const Color Color::PURPLE(1.f, 0.f, 1.f);
const Color Color::CYAN(0.f, 1.f, 1.f);

bool operator ==(const Color& left, const Color& right) {
    return ((left.r == right.r) &&
        (left.b == right.b) &&
        (left.g == right.g) &&
        (left.a == right.a));
}
bool operator !=(const Color& left, const Color& right) {
    return !(left == right);
}
struct FloatRect {
    float x, y, w, h;
    FloatRect() :x(0.f), y(0.f), w(0.f), h(0.f) {}
    FloatRect(float left, float top, float width, float height) :x(left), y(top), w(width), h(height) {}
    FloatRect(glm::vec2 pos, glm::vec2 size) :x(pos.x), y(pos.y), w(size.x), h(size.y) {}
    inline bool contain(const glm::vec2 &point)const {
        return (abs(point.x - x)) <= w && (abs(point.y - y) <= h);
    }
};
struct IntRect {
    int x, y, w, h;
    IntRect() :x(0), y(0), w(0), h(0) {}
    IntRect(int left, int top, int width, int height) :x(left), y(top), w(width), h(height) {}
    IntRect(glm::ivec2 pos, glm::ivec2 size) :x(pos.x), y(pos.y), w(size.x), h(size.y) {}
};
struct Box {
    float 
        left,
        right,
        bottom,
        top,
        near,
        far;
    Box(const FloatRect& rect, float near, float far) :
        left(rect.x), right(rect.w), bottom(rect.h), top(rect.y), near(near), far(far) {}
    Box(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) :
        left(minX), right(maxX), bottom(minY), top(maxY), near(minZ), far(maxZ) {}
    Box() {}
    glm::mat4 ortho()const  {
        return glm::ortho(left, right, bottom, top, near, far);
    }
};
std::ostream& operator<< (std::ostream& out, const Box& box) {
    return out << "left:" << box.left << ", right:" << box.right << ", bottom:" << box.bottom << ", top:" << box.top << ", near:" << box.near << ", far:" << box.far;
}
std::ostream& operator<< (std::ostream& out, const glm::vec3& vec) {
    return out <<"x:"<<vec.x<<", y:"<<vec.y<<", z:"<<vec.z;
}
bool operator ==(const IntRect& left, const IntRect& right) {
    return ((left.x == right.x) &&
            (left.y == right.y) &&
            (left.w == right.w) &&
            (left.h == right.h));
}
bool operator !=(const IntRect& left, const IntRect& right) {
    return !(left == right);
}

struct vec3 :public glm::vec3 {
};
struct vec2 :public glm::vec2 {
};
struct vec4 :public glm::vec4 {
};
struct mat4:public glm::mat4{
};
struct mat3 :public glm::mat3 {
};

glm::vec3 getNormal(const glm::mat3& triangle) {
    return glm::cross(triangle[1] - triangle[2], triangle[0] - triangle[1]);
}
class Clock {
    double last_time, delta_time;
public:
    inline double restart() {
        delta_time = glfwGetTime() - last_time;
        last_time = glfwGetTime();
        return delta_time;
    }
    inline double total() {
        return last_time;
    }
};
#endif