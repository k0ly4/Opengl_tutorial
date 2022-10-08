#ifndef CUBE_H
#define CUBE_H
#include "PrimitiveEntity.h"

class Cube :public Texturable, public Transformable3D {
    
public:

    Cube() {}

    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);

        uniformTransform(shader);
        uniformMaterial(shader);

        sBuffer::cube.getVAO().draw();
    }

private:

};


class gbCube :public Materiable, public Transformable3D {
   
public:

    gbCube() {}

    void draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);

        uniformTransform(shader);
        uniformMaterial(shader);

        sBuffer::cube.getVAO_GB().draw();
    }
private:

};

#endif