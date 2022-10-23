#include "Scene/Primitive.h"

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


