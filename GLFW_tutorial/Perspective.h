#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H
#include "Rects.h"
#include <iostream>
/// <summary>
/// Box
/// </summary>
struct Box {

    union 
    {
        float minX;
        float left;
    };

    union
    {
        float maxX;
        float right;
    };

    union
    {
        float minY;
        float bottom;
    };

    union
    {
        float maxY;
        float top;
    };

    union
    {
        float minZ;
        float near;
    };

    union
    {
        float maxZ;
        float far;  
    };
 
    Box(const FloatRect& rect, float near, float far);
    Box(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
    Box() {}

    glm::mat4 getMatrix()const;

};

std::ostream& operator<< (std::ostream& out, const Box& box);

/// <summary>
/// Perspective
/// </summary>
struct Perspective {
    float near;
    float far;
    float ratio;
    float fov;
    Perspective() {}
    Perspective(float _ratio, float _fov, float _near, float _far);
    glm::mat4 getMatrix()const;
};

inline bool operator ==(const Perspective& left, const Perspective& right) {
    return (left.far == right.far) && (left.near == right.near) && (left.fov == right.fov) && (left.ratio == right.ratio);
}

/// <summary>
/// ProjData
/// </summary>
union ProjData
{
    Box ortho;
    Perspective persp;
    ProjData() {
        std::memset(this, 0, sizeof(ProjData));
    }
};

#endif 
