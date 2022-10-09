#include "Sphere.h"

///////SphereCreator--------------------------------------------
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

///////TextureSphere--------------------------------------------
TextureSphere::TextureSphere() 
{

    VAO.begin();
    VBO.begin();
    EBO.begin();

    VAO.attrib(0, 3, SIZE_UV_VERTEX, 0);
    VAO.attrib(1, 2, SIZE_UV_VERTEX, sizeof(glm::vec3));

    VAO.end();
    VBO.end();
    EBO.end();
}

void TextureSphere::draw(const View* view, const Shader& shader) {
    shader.use();
    view->use(shader);

    uniformTransform(shader);
    uniformMaterial(shader);

    if (needUpBuffers) upBuffers();

    VAO.begin();
    glDrawElements(GlRender::TRIANGLES, countVertex, GL_UNSIGNED_INT, 0);
}

///////Sphere--------------------------------------------

Sphere::Sphere()
{
    VAO.begin();
    VBO.begin();
    EBO.begin();

    VAO.attrib(0, 3, SIZE_VERTEX, 0);
    VAO.attrib(1, 3, SIZE_VERTEX, 3 * sizeof(float));
    VAO.attrib(2, 2, SIZE_VERTEX, 6 * sizeof(float));

    VAO.end();
    VBO.end();
    EBO.end();
}

void Sphere::draw(const View* view, const Shader& shader) {
    shader.use();
    view->use(shader);

    uniformTransform(shader);
    uniformMaterial(shader);

    if (needUpBuffers) {
        upBuffers();
    }
    VAO.begin();
    glDrawElements(GlRender::TRIANGLES, countVertex, GL_UNSIGNED_INT, 0);
}

void Sphere::upBuffers() {

    std::vector<unsigned> indices;
    std::vector<Vertex>vertices;

    SphereCreator::getVertices(vertices, radius_, sectorCount, stackCount);
    VBO.begin();
    VBO.data(vertices);
    VBO.end();

    SphereCreator::getIndices(indices, sectorCount, stackCount);
    EBO.begin();
    EBO.data(indices);
    EBO.end();

    countVertex = indices.size();

    needUpBuffers = 0;
}


