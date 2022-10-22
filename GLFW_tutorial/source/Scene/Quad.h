#ifndef QUAD_H
#define QUAD_H

#include "PrimitiveEntity.h"

///Quad-------------------------------
/// <summary>
/// 
/// </summary>
class Quad :public Drawable {

public:

    Quad() { 
        shaderHint = glShader::frame_exposure;
    }
    void setTexture(const Texture2D& texture) {
        texture_ = &texture;
    }
    void draw(const View* view, const Shader& shader) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        texture_->use(0);
        shader.use();
        shader.uniform("exposure", exposure);

        CullFace::Enable(false);
        sBuffer::quad.getVAO().draw();
    }

private:

    const Texture2D* texture_ =0 ;
    float exposure = 2.2f;
};

///Quad2D-------------------------------
/// <summary>
/// 
/// </summary>
class Quad2D :public Drawable {
    
public:
    Quad2D():
        texture_(0) 
    {
        shaderHint = glShader::frame_exposure;

    }
    void setTexture(const Texture2D& texture) {
       texture_ = &texture;
    }
    void draw(const View* view, const Shader& shader) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        texture_->use(0);
        shader.use();
        shader.uniform("exposure", exposure);
        shader.uniform("model", glm::mat4(1.f));
        CullFace::Enable(false);
        sBuffer::quad2D.getVAO().draw();
    }

private:

    const Texture2D* texture_;
    float exposure = 2.2f;

};

#endif