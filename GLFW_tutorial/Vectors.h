#ifndef VECTORS_H
#define VECTORS_H
#include "GLM.h"
#include <iostream>
struct Vector3f :public glm::vec3 {
};
struct Vector2f :public glm::vec2 {
};
struct Vector4f :public glm::vec4 {
};
struct Matrix4f :public glm::mat4 {
};
struct Matrix3f :public glm::mat3 {
};

std::ostream& operator<< (std::ostream& out, const glm::vec3& vec);
std::ostream& operator<< (std::ostream& out, const glm::mat4& mat);

inline void print(const glm::mat4& mat) {
    for (size_t i = 0; i < 4; i++)
        printf("    %f %f %f %f\n", mat[0][i], mat[1][i], mat[2][i], mat[3][i]);

}
inline glm::vec3 getNormal(const glm::mat3& triangle) {
    return glm::cross(triangle[1] - triangle[2], triangle[0] - triangle[1]);
}
#endif

