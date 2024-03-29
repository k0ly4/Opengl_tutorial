#ifndef MATH_H
#define MATH_H

#include <map>
#include <vector>

#include"Game/Lib/GLFW.h"
#include "Game/Lib/Game.h"
#include "GLM.h"

#include "Math/Color.h"
#include "Math/Rects.h"
#include "Math/Perspective.h"
#include "Math/Vectors.h"
#include "Math/Transform.h"
#include "Math/Matrix.h"

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
/// <param name="power:">���� ���������</param>
/// <param name="min:">�������������� ���� �� ��������� ��������������� ���������</param>
/// <param name="c:"> ��������� ��������� ( c.x * d*d + c.y * d + c.z )</param>
/// <returns></returns>
inline float getLimitPropagation(float power,float min,const glm::vec3& c) {
    return (-c.y + std::sqrtf(c.y * c.y - 4 * c.x * (c.z - power / min))) / (2 * c.x);
    /*return (-c.y + sqrtf(c.y * c.y -
        4 * c.x * (c.x - 256 * power * Light.DiffuseIntensity)))
        /
        (2 * c.x);*/
}

struct Vertex {

    // �������
    glm::vec3 position;

    // �������
    glm::vec3 normal;

    // ���������� ����������
    glm::vec2 texCoord;
    Vertex(const glm::vec3& Position,const glm::vec3& Normal,const glm::vec2& TexCoord):
    position(Position),
    normal(Normal),
    texCoord(TexCoord)
    {}
    Vertex() {}
};
#define SIZE_VERTEX sizeof(Vertex)

///Uint8RGB-------------------------------------------------------
/// <summary>
/// 
/// </summary>
struct Uint8RGB {

    Uint8RGB(byte r_, byte g_, byte b_) :
        r(r_),
        g(g_),
        b(b_)
    {}

    Uint8RGB() :
        r(0),
        b(0),
        g(0)
    {}

    Uint8RGB(byte rgb) :
        r(rgb),
        g(rgb),
        b(rgb)
    {}

    byte r, g, b;
};
#endif