#ifndef CUBE_H
#define CUBE_H
#include "PrimitiveEntity.h"

class Cube :public Drawable, public Transformable3D {
    
public:

    Cube() {
        id_obj = glShader::gb_color_uniform;
    }

    void setColor(const glm::vec3& _color) {
        color = _color;
        texture_ = 0;
        id_obj = glShader::gb_color_uniform;
    }

    void setTexture(Texture2D& texture) {
        id_obj = glShader::gb_texturable;
        texture_ = &texture;
    }

    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        if (texture_) texture_->use(0);
        else shader.uniform("color", color);
        uniformTransform(shader);
        sBuffer::cube.getVAO().draw();
    }

private:

    Texture2D* texture_ = 0;
    glm::vec3 color = glm::vec3(1.f);

};


class gbCube :public gbMateriable, public Transformable3D {
   
public:

    gbCube() {
        id_obj = glShader::gb_color_uniform;
    }

    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        uniformMaterial(shader);
        uniformTransform(shader);
        sBuffer::cube.getVAO_GB().draw();
    }
private:

};

#endif