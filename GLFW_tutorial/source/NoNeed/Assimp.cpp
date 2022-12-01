#include "Assimp.h"

template<typename mat2, typename mat1>
void convertMatrix(size_t size, const mat1& m1, mat2& m2) {
    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++)
            m2[i][j] = m1[j][i];
}

void aiMatrix4x4_to_mat4(const aiMatrix4x4& mat1, glm::mat4& mat2) {
    convertMatrix(4, mat1, mat2);
}

void aiMatrix3x3_to_mat3(const aiMatrix3x3& mat1, glm::mat3& mat2) {
    convertMatrix(3, mat1, mat2);
}

glm::vec3 to_vec3(const aiColor4D& vec) {
    return glm::vec3(vec.r, vec.g, vec.b);
}

glm::vec3 to_vec3(const aiVector3D& vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec2 to_vec2(const aiVector3D& vec) {
    return glm::vec2(vec.x, vec.y);
}

glm::mat4 to_mat4(const aiMatrix4x4& mat) {
    glm::mat4 result;
    aiMatrix4x4_to_mat4(mat, result);
    return result;
}

glm::mat3 to_mat3(const aiMatrix3x3& mat) {
    glm::mat3 result;
    aiMatrix3x3_to_mat3(mat, result);
    return result;
}

void print(const aiMatrix4x4& mat) {
    printf("    %f %f %f %f\n", mat.a1, mat.a2, mat.a3, mat.a4);
    printf("    %f %f %f %f\n", mat.b1, mat.b2, mat.b3, mat.b4);
    printf("    %f %f %f %f\n", mat.c1, mat.c2, mat.c3, mat.c4);
    printf("    %f %f %f %f\n", mat.d1, mat.d2, mat.d3, mat.d4);

}