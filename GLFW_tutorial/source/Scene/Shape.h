//#ifndef SHAPE_H
//#define SHAPE_H
//
//#include "Framebuffer.h"
//#include"Render.h"
//
//
////class Shape:public Drawable, public Transformable {
////
////public:
////
////    Shape() :color(1.f), color_outline(0.f), outlineThickness(1.f), model(1.f) {
////        id_obj = glShader::shape;
////    }
////
////    inline void setFillColor(const Color & color) {
////        this->color = color;
////    }
////
////    inline void setOutlineThickness(float value) {
////        outlineThickness = value;
////        need_update_outline = 1;
////    }
////
////    inline void setOutlineColor(const Color & color) {
////        color_outline = color;
////    }
////
////protected:
////
////    VertexBufferObject VBO;
////    ArrayBufferObject VAO;
////    glm::mat4 model;
////    Color color, color_outline;
////    float outlineThickness;
////    bool need_update_outline = 1;
////
////};
////
/////// <summary>
/////// CircleShape
/////// </summary>
////class CircleShape :public Shape {
////    
////public:
////    CircleShape() {
////        initRenderData(30);
////    }
////    inline void setPointCount(size_t size_vertex) {
////        initRenderData(size_vertex);
////    }
////    inline void setRadius(float radius) {
////        this->radius = radius;
////        need_update_model = 1;
////    }
////    void draw(View* view, Shader& shader);
////
////private:
////    float radius;
////    std::vector<float>vertices;
////
////    void initRenderData(size_t size_vertex);
////    void transform();
////};
////
/////// <summary>
/////// RectangleShape
/////// </summary>
////class RectangleShape :public Shape {
////  
////public:
////
////    RectangleShape() {
////        initFillVertex();
////    }
////
////    void setSize(glm::vec2 size) {
////        local_size = size;
////        need_update_model = 1;
////    }
////
////    void setSize(float x, float y) {
////        setSize(glm::vec2(x, y));
////    }
////
////    void draw(View* view, Shader& shader);
////
////private:
////
////    glm::vec2 local_size, global_size;
////    VertexBufferObject out_VBO;
////    ArrayBufferObject out_VAO;
////    glm::mat4 model_non_scale;
////
////    void transform();
////    void initOutlineVertex();
////    void initFillVertex();
////};
////#endif