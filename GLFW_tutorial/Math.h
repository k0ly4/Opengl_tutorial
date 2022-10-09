#ifndef MATH_H
#define MATH_H

#include"GLFW.h"
#include "GLM.h"

#include <map>
#include <vector>

#include "Color.h"
#include "Rects.h"
#include "Perspective.h"
#include "Vectors.h"
#include "Game.h"
#include "Transform.h"
#include "Matrix.h"

template<typename type>
size_t getMin(size_t size, const type* data) {
    size_t result = 0;
    for (size_t i = 0; i < size - 1; i++)
        if (data[result] > data[i])result = i;

    return result;
}

template<typename type>
type getSum(size_t size, const type* data) {
    type result = data[0];
    for (size_t i = 1; i < size; i++)
        result += data[i];
    return result;
}
/// <summary>
/// 
/// </summary>
/// <param name="power:">Сила источника</param>
/// <param name="min:">Пренебрегаемый ноль на градиенте распространения источника</param>
/// <param name="c:"> Константа затухания ( c.x * d*d + c.y * d + c.z )</param>
/// <returns></returns>
inline float getLimitPropagation(float power,float min,const glm::vec3& c) {
    return (-c.y + std::sqrtf(c.y * c.y - 4 * c.x * (c.z - power / min))) / (2 * c.x);
    /*return (-c.y + sqrtf(c.y * c.y -
        4 * c.x * (c.x - 256 * power * Light.DiffuseIntensity)))
        /
        (2 * c.x);*/
}

struct Vertex {

    // Позиция
    glm::vec3 position;

    // Нормаль
    glm::vec3 normal;

    // Текстурные координаты
    glm::vec2 texCoord;
    Vertex(const glm::vec3& Position,const glm::vec3& Normal,const glm::vec2& TexCoord):
    position(Position),
    normal(Normal),
    texCoord(TexCoord)
    {}
    Vertex() {}
};
#define SIZE_VERTEX sizeof(Vertex)

#endif