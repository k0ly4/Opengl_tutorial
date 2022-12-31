#ifndef SHAPE_H
#define SHAPE_H

#include "Graphic/Drawable.h"
#include "Graphic/Geometry.h"
#include "Resource/ShaderArguments.h"
#include "Math/Transform.h"


class Shape2D:public Drawable {

public:
   
    Shape2D() :
        color(Color::WHITE), 
        color_out(0.f),
        size_out(0.f)
       {}
    Transform2D transform;
protected:

    Color color, color_out;
    float size_out;
    bool modified=1;

};

/// <summary>
/// CircleShape
/// </summary>
class CircleShape :public Shape2D {
    
public:
    CircleShape():
        radius(10.f){
        initRenderData(30);
        transform.setScale(radius, radius);
    }
    inline void setPointCount(size_t size_vertex) {
        initRenderData(size_vertex);
    }
    inline void setFillColor(const Color& color) { this->color = color; }

    inline void setOutlineThickness(float value) {
        size_out = value;
        modified = 1;
    }

    inline void setOutlineColor(const Color& color) { color_out = color; }
    inline void setRadius(float radius) {
        this->radius = radius;
        modified = 1;
    }
    void draw(const View* view, const Shader& shader);
    
private:
    qGeometry<VertexPos2D> mesh;
    float radius;
    void initRenderData(size_t size_vertex);
   // void transform();

};

/// <summary>
/// RectangleShape
/// </summary>
class RectangleShape :public Shape2D {
  
public:

    RectangleShape():
        size_(64.f)
    {shaderHint = glShader::convex2d;}

    void setSize(glm::vec2 size) {
        size_ = size;
        modified = 1;
    }
    inline void setFillColor(const Color& color) { 
        this->color = color;
        modified = 1;
    }

    inline void setOutlineThickness(float value) {
        size_out = value;
        modified = 1;
    }

    inline void setOutlineColor(const Color& color) { 
        color_out = color; 
        modified = 1;
    }
    void setSize(float x, float y) {setSize({ x, y });}

    void draw(const View* view, const Shader& shader);

private:

    iGeometry<VertexConvex2D> mesh;
    glm::vec2 size_;
    void solve();

};
#endif