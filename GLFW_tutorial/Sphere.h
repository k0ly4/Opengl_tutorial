#ifndef SPHERE_H
#define SPHERE_H
#include "PrimitiveEntity.h"
#include "Convex.h"
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

///////GeneralSphere--------------------------------------------
class GeneralSphere: public Transformable3D {

public:

    GeneralSphere():
        radius_(1.f),
        sectorCount(6),
        stackCount(6),
        countVertex(0) {}

    void setRadius(float radius) {
        radius_ = radius;
        needUpBuffers = 1;
    }

    void setCountStack(size_t count) {
        if (sectorCount == count)return;
        sectorCount = count;
        needUpBuffers = 1;
    }

    void setCountSector(size_t count) {
        if (stackCount == count)return;
        stackCount = count;
        needUpBuffers = 1;
    }

    void displayLine(RenderTarget& target) {
        const Shader& shader = glShader::get(glShader::texture);
        BasisMaterial material(Color::YELLOW);

        shader.use();
        target.getView()->use(shader);
        uniformTransform(shader);

        shader.uniform(material);
        if (needUpBuffers) upBuffers();

        VAO.begin();
        glDrawElements(GlRender::LINE_STRIP, countVertex, GL_UNSIGNED_INT, 0);
    }

protected :

    virtual void upBuffers() = 0;

    size_t countVertex;
    size_t sectorCount;
    size_t stackCount;

    bool needUpBuffers = 1;
    float radius_;

    VertexBufferObject VBO;
    ElementBufferObject EBO;
    ArrayBufferObject VAO;
};

///////LightSphere--------------------------------------------

class LightSphere : public GeneralSphere {

public:

    LightSphere()
   {
        VAO.begin();
        VBO.begin();
        EBO.begin();
        VAO.attrib(0, 3, sizeof(glm::vec3), 0);
        VAO.end();
        VBO.end();
        EBO.end();
    }
    ///without material
    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        uniformTransform(shader);

        if (needUpBuffers) {
            upBuffers();
        }

        VAO.begin();
        glDrawElements(GlRender::TRIANGLES, countVertex, GL_UNSIGNED_INT, 0);
    }

protected:

    void upBuffers() {

        std::vector<unsigned> indices;
        std::vector<glm::vec3>vertices;

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
};
///////TextureSphere--------------------------------------------
class TextureSphere :public Texturable,public GeneralSphere {

public:

    TextureSphere();

    void draw(const View* view, const Shader& shader);

    DrawBuffer getVAO()const {
        return DrawBuffer(VAO, DataDraw(DataDraw::DrawElements, GlRender::TRIANGLES, countVertex));
    }
private:

    void upBuffers() {

        std::vector<unsigned> indices;
        std::vector<UvVertex>vertices;

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

};

///////Sphere--------------------------------------------
class Sphere :public Materiable, public GeneralSphere {

public:

    Sphere();

    void draw(const View* view, const Shader& shader);

private:

    void upBuffers();
};
#endif