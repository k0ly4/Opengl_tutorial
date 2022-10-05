#ifndef RECTS_H
#define RECTS_H

#include "GLM.h"

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
