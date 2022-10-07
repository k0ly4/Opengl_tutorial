#pragma once
#include "Vectors.h"
std::ostream& operator<< (std::ostream& out, const glm::vec3& vec) {
    return out << "x:" << vec.x << ", y:" << vec.y << ", z:" << vec.z;
}
std::ostream& operator<< (std::ostream& out, const glm::vec4& vec) {
    return out << "x:" << vec.x << ", y:" << vec.y << ", z:" << vec.z<< ", w:" << vec.w;
}
std::ostream& operator<< (std::ostream& out, const glm::mat4& mat) {
    for (size_t i = 0; i < 4; i++) {
        out << '\n';
        for (size_t j = 0; j < 4; j++)
            out << mat[i][j] << ' ';
    }
    return out;
}