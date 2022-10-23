#ifndef SPHERE_H
#define SPHERE_H

#include "PrimitiveEntity.h"
#include "Convex.h"

///////Cube--------------------------------------------
class CubeCreator {
public:
    static void generate(std::vector<Vertex>& vertices, float size_);
private:
    CubeCreator() {}
    ~CubeCreator() {}
};

///////Sphere--------------------------------------------
class SphereCreator {

    public:
        static void getVertices(std::vector < glm::vec3 > & vertices, float radius_, size_t sectorCount, size_t stackCount);
        static void getVertices(std::vector<UvVertex>& vertices, float radius_, size_t sectorCount, size_t stackCount);
        static void getVertices(std::vector<Vertex>& vertices, float radius_, size_t sectorCount, size_t stackCount);

        static void getIndices(std::vector<unsigned>& indices, size_t sectorCount, size_t stackCount);
private:

    SphereCreator() {}
    ~SphereCreator() {}
};


#endif