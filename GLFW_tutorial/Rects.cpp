#pragma once

#include "Rects.h"


FloatRect::FloatRect() :x(0.f), y(0.f), w(0.f), h(0.f) {}
FloatRect::FloatRect(float left, float top, float width, float height) :x(left), y(top), w(width), h(height) {}
FloatRect::FloatRect(glm::vec2 pos, glm::vec2 size) :x(pos.x), y(pos.y), w(size.x), h(size.y) {}

bool FloatRect::contain(const glm::vec2& point)const {
    return (abs(point.x - x)) <= w && (abs(point.y - y) <= h);
}

bool operator ==(const FloatRect& left, const FloatRect& right) {
    return ((left.x == right.x) &&
        (left.y == right.y) &&
        (left.w == right.w) &&
        (left.h == right.h));
}




IntRect::IntRect() :x(0), y(0), w(0), h(0) {}
IntRect::IntRect(int left, int top, int width, int height) :x(left), y(top), w(width), h(height) {}
IntRect::IntRect(glm::ivec2 pos, glm::ivec2 size) :x(pos.x), y(pos.y), w(size.x), h(size.y) {}

bool operator ==(const IntRect& left, const IntRect& right) {
    return ((left.x == right.x) &&
        (left.y == right.y) &&
        (left.w == right.w) &&
        (left.h == right.h));
}

