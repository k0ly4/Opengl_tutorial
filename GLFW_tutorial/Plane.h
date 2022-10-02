#ifndef PLANE_H
#define PLANE_H

#include "PrimitiveEntity.h"

class Plane :public Drawable, public Transformable3D {
   
public:
   
    Plane() {
        id_obj = glShader::m_texture;
    }

    void setTexture(Texture2D& texture) {
        this->texture = &texture;
    }

    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        uniformTransform(shader);
        texture->use(0);
        sBuffer::plane.getVAO().draw();
    }

private:

    
    Texture2D* texture;
};
class gbPlane :public gbMateriable, public Transformable3D {
   
public:

    gbPlane() {
        id_obj = glShader::gb_color_uniform;
    }

    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        uniformMaterial(shader);
        uniformTransform(shader);
        sBuffer::plane.getVAO_gb().draw();
    }

private:
};
#endif