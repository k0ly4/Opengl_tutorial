#pragma once

#include "Perspective.h"

//class Box
Box::Box(const FloatRect& rect, float near, float far) :
        left(rect.x), right(rect.w), bottom(rect.h), top(rect.y), near(near), far(far) {}
Box::Box(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) :
        left(minX), right(maxX), bottom(minY), top(maxY), near(minZ), far(maxZ) {}

glm::mat4 Box::getMatrix()const {
        return glm::ortho(left, right, bottom, top, near, far);
}

std::ostream& operator<< (std::ostream& out, const Box& box) {
    return out << "left:" << box.left << ", right:" << box.right << ", bottom:" << box.bottom << ", top:" << box.top << ", near:" << box.near << ", far:" << box.far;
}

//class Perspective
Perspective::Perspective(float _ratio, float _fov, float _near, float _far) :ratio(_ratio), fov(_fov), near(_near), far(_far) {}

glm::mat4 Perspective::getMatrix()const {
        return glm::perspective(glm::radians(fov), ratio, near, far);
    }

