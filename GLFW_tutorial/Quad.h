#ifndef QUAD_H
#define QUAD_H
#include "PrimitiveEntity.h"

class Quad :public Drawable {

public:

    Quad() { 
        id_obj = glShader::frame_exposure;
    }
    void setTexture(Texture2D& texture) {
        this->texture = &texture;
    }
    void draw(const View* view, const Shader& shader) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        texture->use(0);
        shader.use();
        shader.uniform("exposure", exposure);

        GlRender::CullFace::Enable(false);
        sBuffer::quad.getVAO().draw();
    }

private:

    Texture2D* texture;
    float exposure = 2.2f;
};


class Quad2D :public Drawable {
    
public:
    Quad2D() {
        id_obj = glShader::frame_exposure;

    }
    void setTexture(Texture2D& texture) {
        this->texture = &texture;
    }
    void draw(const View* view, const Shader& shader) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        texture->use(0);
        shader.use();
        shader.uniform("exposure", exposure);

        GlRender::CullFace::Enable(false);
        sBuffer::quad2D.getVAO().draw();
    }

private:
    Texture2D* texture;
    float exposure = 2.2f;
};

#endif