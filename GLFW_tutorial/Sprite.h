
#ifndef SPRITE_H
#define SPRITE_H

#include"Texture.h"
#include"Render.h"
#include "PrimitiveEntity.h"
class Sprite :public Drawable, public Transformable {
    
public:

    Sprite() : 
        color_(1.f), 
        model_uv(1.f), 
        model(1.f),
        size_(0.f)
    {
        id_obj = glShader::sprite;
    }

    inline void setColor(const Color& color) {
        color_ = color;
    }
    void setTexture(const Texture2D& texture);
    void setTextureRect(const FloatRect& cut_rect);

    void draw(const View* view,const Shader& shader);

private:

    glm::vec2 size_;
    Color color_;
    glm::mat4 model, model_uv; 
    const Texture2D* texture_;

    void transform();
};

#endif