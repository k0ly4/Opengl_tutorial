#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H
#include "Rects.h"
#include <iostream>
struct Box {
    float
        left,
        right,
        bottom,
        top,
        near,
        far;
    Box(const FloatRect& rect, float near, float far);
    Box(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
    Box() {}
    glm::mat4 getMatrix()const;
};
std::ostream& operator<< (std::ostream& out, const Box& box);

struct Perspective {
    float near;
    float far;
    float ratio;
    float fov;
    Perspective() {}
    Perspective(float _ratio, float _fov, float _near, float _far);
    glm::mat4 getMatrix()const;
};

union ProjData
{
    Box ortho;
    Perspective persp;
    ProjData() {
        std::memset(this, 0, sizeof(ProjData));
    }
};
#endif 
