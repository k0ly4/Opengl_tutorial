#ifndef ASSIMP_LIB_H
#define ASSIMP_LIB_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Math.h"

void aiMatrix4x4_to_mat4(const aiMatrix4x4& mat1, glm::mat4& mat2);

void aiMatrix3x3_to_mat3(const aiMatrix3x3& mat1, glm::mat3& mat2);

glm::vec3 to_vec3(const aiColor4D& vec);

glm::vec3 to_vec3(const aiVector3D& vec);

glm::vec2 to_vec2(const aiVector3D& vec);

glm::mat4 to_mat4(const aiMatrix4x4& mat);

glm::mat3 to_mat3(const aiMatrix3x3& mat);

void print(const aiMatrix4x4& mat);
#endif
