
#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "Sphere.h"

///////Cube--------------------------------------------
class Cube :public Transformable3D, public Drawable{

public:

    Cube() {
        shaderHint = glShader::texture_loc2;
        graphic = std::make_shared<Graphic>();
    }

    Cube(glShader::Object shaderHint_) {
        shaderHint = shaderHint_;
        graphic = std::make_shared<Graphic>();
    }
    void genMesh(float size) {
        std::vector<Vertex> vertices;
        CubeCreator::generate(vertices, size);
        graphic->setMesh(vertices);
    }

    std::shared_ptr<Graphic>& getGraphic() {
        return graphic;
    }

    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        uniformTransform(shader);
        graphic->draw(shader);
    }

private:

    std::shared_ptr<Graphic> graphic;

};

///////GeneralSphere--------------------------------------------
class GeneralSphere : public Transformable3D {

public:

    GeneralSphere() :
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

protected:

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

///////TextureSphere--------------------------------------------
class TextureSphere :public Texturable, public GeneralSphere {

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