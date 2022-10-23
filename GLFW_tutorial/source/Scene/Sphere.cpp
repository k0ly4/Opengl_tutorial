#include "Sphere.h"
///////CubeCreator-------------------------------------------
void CubeCreator::generate(std::vector<Vertex>& vertices, float size_) {
    vertices.resize(36);
    // back face
    vertices[0] = Vertex(glm::vec3(-size_, -size_, -size_),    glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2( 0.0f, 0.0f));
    vertices[1] = Vertex(glm::vec3( size_,  size_, -size_),    glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2( 1.0f, 1.0f));
    vertices[2] = Vertex(glm::vec3( size_, -size_, -size_),    glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2( 1.0f, 0.0f));
    vertices[3] = Vertex(glm::vec3( size_,  size_, -size_),    glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2( 1.0f, 1.0f));
    vertices[4] = Vertex(glm::vec3(-size_, -size_, -size_),    glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2( 0.0f, 0.0f));
    vertices[5] = Vertex(glm::vec3(-size_,  size_, -size_),    glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2( 0.0f, 1.0f));
    // front face
    vertices[6] =  Vertex(glm::vec3(-size_, -size_,  size_),    glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f));
    vertices[7] =  Vertex(glm::vec3( size_, -size_,  size_),    glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.0f));
    vertices[8] =  Vertex(glm::vec3( size_,  size_,  size_),    glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f));
    vertices[9] =  Vertex(glm::vec3( size_,  size_,  size_),    glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f));
    vertices[10] = Vertex(glm::vec3(-size_,  size_,  size_),    glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 1.0f));
    vertices[11] = Vertex(glm::vec3(-size_, -size_,  size_),    glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f));
    // left face
    vertices[12] = Vertex(glm::vec3(-size_,  size_,  size_),    glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f));
    vertices[13] = Vertex(glm::vec3(-size_,  size_, -size_),    glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f));
    vertices[14] = Vertex(glm::vec3(-size_, -size_, -size_),    glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f));
    vertices[15] = Vertex(glm::vec3(-size_, -size_, -size_),    glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f));
    vertices[16] = Vertex(glm::vec3(-size_, -size_,  size_),    glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f));
    vertices[17] = Vertex(glm::vec3(-size_,  size_,  size_),    glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f));
    // right face
    vertices[18] = Vertex(glm::vec3(size_,  size_,  size_),     glm::vec3(1.0f,  0.0f,  0.0f),  glm::vec2(1.0f, 0.0f));
    vertices[19] = Vertex(glm::vec3(size_, -size_, -size_),     glm::vec3(1.0f,  0.0f,  0.0f),  glm::vec2(0.0f, 1.0f));
    vertices[20] = Vertex(glm::vec3(size_,  size_, -size_),     glm::vec3(1.0f,  0.0f,  0.0f),  glm::vec2(1.0f, 1.0f));
    vertices[21] = Vertex(glm::vec3(size_, -size_, -size_),     glm::vec3(1.0f,  0.0f,  0.0f),  glm::vec2(0.0f, 1.0f));
    vertices[22] = Vertex(glm::vec3(size_,  size_,  size_),     glm::vec3(1.0f,  0.0f,  0.0f),  glm::vec2(1.0f, 0.0f));
    vertices[23] = Vertex(glm::vec3(size_, -size_,  size_),     glm::vec3(1.0f,  0.0f,  0.0f),  glm::vec2(0.0f, 0.0f));
    // bottom face
    vertices[24] = Vertex(glm::vec3( -size_, -size_, -size_),    glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2( 0.0f, 1.0f));
    vertices[25] = Vertex(glm::vec3(  size_, -size_, -size_),    glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2( 1.0f, 1.0f));
    vertices[26] = Vertex(glm::vec3(  size_, -size_,  size_),    glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2( 1.0f, 0.0f));
    vertices[27] = Vertex(glm::vec3(  size_, -size_,  size_),    glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2( 1.0f, 0.0f));
    vertices[28] = Vertex(glm::vec3( -size_, -size_,  size_),    glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2( 0.0f, 0.0f));
    vertices[29] = Vertex(glm::vec3( -size_, -size_, -size_),    glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2( 0.0f, 1.0f));
    // top face                     
    vertices[30] = Vertex(glm::vec3( -size_,  size_, -size_),    glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f));
    vertices[31] = Vertex(glm::vec3(  size_,  size_ , size_),    glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f));
    vertices[32] = Vertex(glm::vec3(  size_,  size_, -size_),    glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f));
    vertices[33] = Vertex(glm::vec3(  size_,  size_,  size_),    glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f));
    vertices[34] = Vertex(glm::vec3( -size_,  size_, -size_),    glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f));
    vertices[35] = Vertex(glm::vec3( -size_,  size_,  size_),    glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f));
}
///////SphereCreator-------------------------------------------
void SphereCreator::getVertices(std::vector<glm::vec3>& vertices, float radius_, size_t sectorCount, size_t stackCount) {

    vertices.resize((stackCount + 1) * (sectorCount + 1));

    float x, y, z, xy;                              // vertex position

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    size_t index = 0;
    for (size_t i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius_ * cosf(stackAngle);             // r * cos(u)
        z = radius_ * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (size_t j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            vertices[index] = glm::vec3(x, y, z);
               
            index++;
        }
    }

}

void SphereCreator::getVertices(std::vector<UvVertex>& vertices, float radius_, size_t sectorCount, size_t stackCount) {

    vertices.resize((stackCount + 1) * (sectorCount + 1));

    float x, y, z, xy;                              // vertex position
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    size_t index = 0;
    for (size_t i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius_ * cosf(stackAngle);             // r * cos(u)
        z = radius_ * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (size_t j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices[index] = UvVertex(
                glm::vec3(x, y, z),
                glm::vec2(s, t));
            index++;
        }
    }

}

void SphereCreator::getVertices(std::vector<Vertex>& vertices, float radius_, size_t sectorCount, size_t stackCount) {

    vertices.resize((stackCount + 1) * (sectorCount + 1));

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius_;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    size_t index = 0;
    for (size_t i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius_ * cosf(stackAngle);             // r * cos(u)
        z = radius_ * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (size_t j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices[index] = Vertex(
                glm::vec3(x, y, z),
                glm::vec3(nx, ny, nz),
                glm::vec2(s, t));
            index++;
        }
    }

}

void SphereCreator::getIndices(std::vector<unsigned>& indices, size_t sectorCount, size_t stackCount) {
    indices.clear();

    int k1, k2;
    for (size_t i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (size_t j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

        }
    }
}

