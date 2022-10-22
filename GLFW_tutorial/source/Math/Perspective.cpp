#pragma once

#include "Perspective.h"

//class Box
Box::Box(const FloatRect& rect, float near, float far) :
        left(rect.x), right(rect.w), bottom(rect.h), top(rect.y), near(near), far(far) {}
Box::Box(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) :
        left(minX), right(maxX), bottom(minY), top(maxY), near(minZ), far(maxZ) {}



std::ostream& operator<< (std::ostream& out, const Box& box) {
    return out << "left:" << box.left << ", right:" << box.right << ", bottom:" << box.bottom << ", top:" << box.top << ", near:" << box.near << ", far:" << box.far;
}



