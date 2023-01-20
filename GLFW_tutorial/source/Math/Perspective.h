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

    //no dirty flag
    inline glm::mat4 getMatrix()const {
        return glm::ortho(left, right, bottom, top, near, far);
    }
};

std::ostream& operator<< (std::ostream& out, const Box& box);

/// <summary>
/// Perspective
/// </summary>
struct Perspective {

    float near;
    float far;
    float ratio;
    //in degres
    float fov;

    Perspective() :
        ratio(1.f),
        fov(1.f),
        near(0.f),
        far(0.f) {}

    Perspective(float _ratio, float _fov, float _near, float _far) :
        ratio(_ratio),
        fov(_fov),
        near(_near),
        far(_far) {}

    //no dirty flag
    inline glm::mat4 getMatrix()const {
        return glm::perspective(glm::radians(fov), ratio, near, far);
    }
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
