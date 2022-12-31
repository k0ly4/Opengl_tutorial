#ifndef PLANE_H
#define PLANE_H

#include "PrimitiveEntity.h"
#include "Transformable.h"
#include "Graphic/Drawable.h"

class Plane :public Texturable, public Transformable3D {
   
public:
   
    Plane() 
    {}

    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        uniformTransform(shader);
        uniformMaterial(shader);
        sBuffer::plane->getVAO().draw();
    }

private:

};
class gbPlane :public Materiable, public Transformable3D {
   
public:

    gbPlane() {}

    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        uniformTransform(shader);
        uniformMaterial(shader);
        sBuffer::plane->getVAO_gb().draw();
    }

private:
};
#endif