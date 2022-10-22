#ifndef RECTS_H
#define RECTS_H

#include "GLM.h"
#include <iostream>

////Line------------------------------------------------
class CornerRect {
public:

    glm::vec2 lu;//left and up
    glm::vec2 rd;//right and down

    CornerRect(float leftUpX, float leftUpY, float rightDownX, float rightDownY) :
        CornerRect(glm::vec2(leftUpX,leftUpY),glm::vec2(rightDownX,rightDownY))
    {}
    CornerRect(const glm::vec2& leftUp, const glm::vec2& rightDown) :
        lu(leftUp), rd(rightDown) 
    {}
    CornerRect() :
        CornerRect(glm::vec2(0.f),glm::vec2(0.f))
    {}

};

inline CornerRect operator /(const CornerRect& a, const glm::vec2& scale) {
    return CornerRect(a.lu / scale, a.rd/scale);
}

inline CornerRect operator *(const CornerRect& a, const glm::vec2& scale) {
    return CornerRect(a.lu * scale, a.rd * scale);
}
////FloatRect------------------------------------------------
class FloatRect {
public:
    float x, y, w, h;
    FloatRect();
    FloatRect(float def) :
        x(def), y(def), w(def), h(def) 
    {}
    FloatRect(float left, float top, float width, float height);
    FloatRect(const glm::vec2& pos, float width, float height) :
        FloatRect(pos.x, pos.y, width, height) {}
    FloatRect(float left, float top, glm::vec2 size) :
        FloatRect(left,top, size.x,size.y) {}
    FloatRect(glm::vec2 pos, glm::vec2 size);
    bool contain(const glm::vec2& point)const;
    inline glm::vec4 vec4()const {
        return glm::vec4(x, y, w, h);
    }
private:
};
bool operator ==(const FloatRect& left, const FloatRect& right);

inline FloatRect operator *(const FloatRect& left, float value) {
    return FloatRect(left.x * value, left.y * value, left.w * value, left.h * value);
}
inline FloatRect operator /(const FloatRect& left, float value) {
    return FloatRect(left.x / value, left.y / value, left.w / value, left.h / value);
}
inline FloatRect operator *(const FloatRect& left, const glm::vec2& size) {
    return FloatRect(left.x * size.x, left.y * size.y, left.w * size.x, left.h * size.y);
}
inline FloatRect operator /(const FloatRect& left, const glm::vec2& size) {
    return FloatRect(left.x / size.x, left.y / size.y, left.w / size.x, left.h / size.y);
}
inline bool operator !=(const FloatRect& left, const FloatRect& right) {
    return !(left == right);
}

inline std::ostream& operator<< (std::ostream& out, const FloatRect& rect) {
    out << rect.x << ' ' << rect.y << ' ' << rect.w << ' ' << rect.h;
    return out;
}

////IntRect------------------------------------------------
class IntRect {
public:
    int x, y, w, h;
    IntRect();
    IntRect(int left, int top, int width, int height);
    IntRect(glm::ivec2 pos, glm::ivec2 size);
private:
};

bool operator ==(const IntRect& left, const IntRect& right);
inline bool operator !=(const IntRect& left, const IntRect& right) {
    return !(left == right);
}
#endif 
