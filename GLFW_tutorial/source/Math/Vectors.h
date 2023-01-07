#ifndef VECTORS_H
#define VECTORS_H

#include "GLM.h"
#include <iostream>
#include <vector>

inline void print(const glm::mat4& mat) {
    for (size_t i = 0; i < 4; i++)
        printf("    %f %f %f %f\n", mat[0][i], mat[1][i], mat[2][i], mat[3][i]);

}

inline glm::vec3 getNormal(const glm::mat3& triangle) {return glm::cross(triangle[1] - triangle[2], triangle[0] - triangle[1]);}

static glm::vec3 getCenter(const std::vector<glm::vec4>& vertices) {
    glm::vec3 center(0.f);
    for (size_t i = 0; i < vertices.size(); i++)
    {
        center += glm::vec3(vertices[i]);
    }
    center /= vertices.size();
    return center;
}
///Vec4M---------------------------------------------
struct Vec4Array {
    union
    {
        struct { glm::vec4 v; };
        struct { float m[4]; };
    };
    Vec4Array(const glm::vec4& v_) :v(v_) {}
    Vec4Array(){}
};
#endif

