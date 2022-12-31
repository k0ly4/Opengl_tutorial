#ifndef SHADER_ARGUMENTS_H
#define SHADER_ARGUMENTS_H

#include "Scene/PrimitiveEntity.h"
/// <summary>
/// VertexPos2D
/// </summary>
template <typename T1>
class Vertex1 {
public:
    T1 v1;
    inline static void attrib(ArrayBufferObject& VAO) { VAO.attrib(0, sizeof(T1) / sizeof(float), sizeof(Vertex1<T1>), 0); }
    Vertex1(const T1& v1_) :v1(v1_) {}
    Vertex1() :v1(0){}
};
template <typename T1,typename T2>
class Vertex2 {
public:
    T1 v1;
    T2 v2;
    inline static void attrib(ArrayBufferObject& VAO) {
        VAO.attrib(0, sizeof(T1) / sizeof(float), sizeof(Vertex2<T1,T2>), 0);
        VAO.attrib(1, sizeof(T2) / sizeof(float), sizeof(Vertex2<T1,T2>), sizeof(T1));
    }
    Vertex2(const T1& v1_, const T2& v2_) :
        v1(v1_),
        v2(v2_) {}
    Vertex2() :
        v1(0),
        v2(0) {}
};
/// <summary>
/// VertexPos2D
/// </summary>
struct VertexPos2D{
    #define SIZE_VERTEX_2D_POS_ sizeof(VertexPos2D)
    glm::vec2 pos;
    static void attrib(ArrayBufferObject& VAO) {VAO.attrib(0, 2, SIZE_VERTEX_2D_POS_, 0);}
    VertexPos2D() :pos(0.f) {}
    VertexPos2D(const glm::vec2& pos_) :pos(pos_){}
};
/// <summary>
/// VertexPos2D
/// </summary>
struct VertexConvex2D {
#define SIZE_VERTEX_CONVEX_2D sizeof(VertexConvex2D)
    glm::vec2 pos;
    glm::vec4 color;
    static void attrib(ArrayBufferObject& VAO) { 
        VAO.attrib(0, 2, SIZE_VERTEX_CONVEX_2D, 0);
        VAO.attrib(1, 4, SIZE_VERTEX_CONVEX_2D, sizeof(glm::vec2));
    }
    VertexConvex2D() :pos(0.f),color(0.f) {}
    VertexConvex2D(glm::vec2 pos_,const glm::vec4& color_) :pos(pos_),color(color_) {}
    VertexConvex2D(glm::vec2 pos_, const Color& color_) :pos(pos_), color(color_.vec4()) {}
};
/// <summary>
/// ConvexVertex
/// </summary>
struct ConvexVertex {

    glm::vec3 pos;
    glm::vec4 color;
#define SIZE_CONVEX_VERTEX sizeof(ConvexVertex)

    static void attrib(ArrayBufferObject& VAO) {
        VAO.attrib(0, 3, SIZE_CONVEX_VERTEX, 0);
        VAO.attrib(1, 4, SIZE_CONVEX_VERTEX, sizeof(glm::vec3));
    }
    ConvexVertex() :
        pos(0.f),
        color(1.f) {}

    ConvexVertex(const glm::vec3& pos_, const glm::vec4& color_) :
        pos(pos_),
        color(color_)
    {}
};

struct UvVertex {
    glm::vec3 pos;
    glm::vec2 uv;
#define SIZE_UV_VERTEX sizeof(UvVertex)
    UvVertex() :
        pos(glm::vec3(0.f)),
        uv(glm::vec3(0.f))
    {}
    UvVertex(const glm::vec3& pos_, const glm::vec2& uv_) :
        pos(pos_),
        uv(uv_) {}

    static void attrib(ArrayBufferObject& VAO) {
        VAO.attrib(0, 3, SIZE_UV_VERTEX, 0);
        VAO.attrib(1, 2, SIZE_UV_VERTEX, sizeof(glm::vec3));
    }
};


struct VoxelVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec4 light;
#define SIZE_VOXEL_VERTEX sizeof(VoxelVertex)
    VoxelVertex() :
        pos(0.f),
        uv(0.f),
        light(0.f)
    {}
    VoxelVertex(const glm::vec3& pos_, const glm::vec2& uv_, const glm::vec4& light_) :
        pos(pos_),
        uv(uv_),
        light(light_)
    {}

    static void attrib(ArrayBufferObject& VAO) {
        VAO.attrib(0, 3, SIZE_VOXEL_VERTEX, 0);
        VAO.attrib(1, 2, SIZE_VOXEL_VERTEX, sizeof(glm::vec3));
        VAO.attrib(2, 4, SIZE_VOXEL_VERTEX, sizeof(glm::vec2) + sizeof(glm::vec3));
    }
};

struct SquareUV {
    UvVertex vertices[4];
    SquareUV(const UvVertex& lt, const UvVertex& ld, const UvVertex& rt, const UvVertex& rd)
        //:vertices{ lt, ld, rt, rd } 
    {
        vertices[0] = lt;
        vertices[1] = ld;
        vertices[2] = rt;
        vertices[3] = rd;
    }
};
#endif