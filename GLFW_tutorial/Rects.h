#ifndef RECTS_H
#define RECTS_H

#include "GLM.h"

class FloatRect {
public:
    float x, y, w, h;
    FloatRect();
    FloatRect(float left, float top, float width, float height);
    FloatRect(const glm::vec2& pos, float width, float height) :
        FloatRect(pos.x, pos.y, width, height) {}
    FloatRect(float left, float top, glm::vec2 size) :
        FloatRect(left,top, size.x,size.y) {}
    FloatRect(glm::vec2 pos, glm::vec2 size);
    bool contain(const glm::vec2& point)const;
private:
};

class IntRect {
public:
    int x, y, w, h;
    IntRect();
    IntRect(int left, int top, int width, int height);
    IntRect(glm::ivec2 pos, glm::ivec2 size);
private:
};

bool operator ==(const IntRect& left, const IntRect& right);
bool operator !=(const IntRect& left, const IntRect& right);

#endif 
