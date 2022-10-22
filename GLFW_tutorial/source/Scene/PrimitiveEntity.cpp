#include "PrimitiveEntity.h"

//Quad
//
//VAO
void sBuffer::Quad2D::initializeVAO() {

    VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLE_STRIP, 4);
    VAO.begin();
    VAO.attrib(0, 4, 4 * sizeof(float), 0);
}
//VBO
void sBuffer::Quad2D::initializeVBO(){

    float vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f
    };
    VBO_.data(sizeof(vertices), vertices);
}

sBuffer::Quad2D sBuffer::quad2D;


//Quad2D
// 
//VAO
void sBuffer::Quad::initializeVAO() {

    VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLE_STRIP, 4);
    VAO.begin();
    VAO.attrib(0, 3, 5 * sizeof(float), 0);
    VAO.attrib(1, 2, 5 * sizeof(float), 3 * sizeof(float));

}
//VBO
void sBuffer::Quad::initializeVBO() {

    float vertices[] = {
        // координаты      // текстурные координаты
       -1.0f,  1.0f, 0.0f,      0.0f, 1.0f,
       -1.0f, -1.0f, 0.0f,      0.0f, 0.0f,
        1.0f,  1.0f, 0.0f,      1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
    };
    VBO_.data(sizeof(vertices), vertices);
}

sBuffer::Quad sBuffer::quad;


//Cube
//
//VAO
void sBuffer::Cube::initializeVAO() {

    VAO_gb.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLES, 36);
    VAO_gb.begin();
    VAO_gb.attrib(0, 3, 8 * sizeof(float), 0);
    VAO_gb.attrib(1, 3, 8 * sizeof(float), 3 * sizeof(float));
    VAO_gb.attrib(2, 2, 8 * sizeof(float), 6 * sizeof(float));

    VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLES, 36);
    VAO.begin();
    VAO.attrib(0, 3, 8 * sizeof(float), 0);
    VAO.attrib(1, 2, 8 * sizeof(float), 6 * sizeof(float));
}
//VBO
void sBuffer::Cube::initializeVBO() {
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
         // bottom face
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
          1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         // top face
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
          1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
          1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
          1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };
    VBO_.data(sizeof(vertices), vertices);
}

sBuffer::Cube sBuffer::cube;

//Plane
//
//VAO
void sBuffer::Plane::initializeVAO() {
    VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLE_STRIP, 4);
    VAO.begin();
    VAO.attrib(0, 3, 8 * sizeof(float), 0);
    VAO.attrib(1, 2, 8 * sizeof(float), 6 * sizeof(float));

    VAO_gb.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLE_STRIP, 4);
    VAO_gb.begin();
    VAO_gb.attrib(0, 3, 8 * sizeof(float), 0);
    VAO_gb.attrib(1, 3, 8 * sizeof(float), 3 * sizeof(float));
    VAO_gb.attrib(2, 2, 8 * sizeof(float), 6 * sizeof(float));
}
//VBO
void sBuffer::Plane::initializeVBO() {
    float vertices[] = {
        // positions            // normals         // texcoords
        -0.5f,  -0.5f,0.0f, 0.f,1.f,0.f,       0.0f,  0.0f,
        0.5f, -0.5f,  0.0f, 0.f,1.f,0.f,      1.0f,  0.0f,
        -0.5f, 0.5f,  0.0f, 0.f,1.f,0.f,       0.0f,  1.0f,
        0.5f,  0.5f,  0.0f, 0.f,1.f,0.f,       1.0f,  1.0f
    };
    VBO_.data(sizeof(vertices), vertices);
}

sBuffer::Plane sBuffer::plane;




