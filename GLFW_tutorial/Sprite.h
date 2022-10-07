
#ifndef SPRITE_H
#define SPRITE_H

#include"Texture.h"
#include"Render.h"
#include "PrimitiveEntity.h"
#include "Convex.h"

class Sprite :public Drawable, public Transformable2D {
    
public:

    Sprite() :
        convex_(4),
        texture_(0),
        color_(1.f), 
        textureRect_(0.f)
    {
        id_obj = glShader::main_texture;
    }
    Sprite(const Texture2D& texture) :
        Sprite()
    {
        setTexture(texture);
    }

    inline void setColor(const Color& color) {
        color_ = color;
    }
    void setTexture(const Texture2D& texture);
    void setTextureRect(const FloatRect& cut_rect);
    const FloatRect& getTextureRect() const {
        return textureRect_;
    }
    void draw(const View* view,const Shader& shader);

private:

    void upConvex() {
        FloatRect normalize = textureRect_ / texture_->getSize();

        normalize.w = normalize.x + normalize.w;
        //flip vertically
        normalize.y = 1.f - normalize.y;
        normalize.h = normalize.y - normalize.h;
        
        convex_[0] = UvVertex(glm::vec3(0.f,            textureRect_.h, 0.0f), glm::vec2(normalize.x, normalize.h));      //glm::vec2(0.f,0.f)
        convex_[1] = UvVertex(glm::vec3(0.f,            0.f,            0.0f), glm::vec2(normalize.x, normalize.y));      //glm::vec2(0.f,1.f)
        convex_[2] = UvVertex(glm::vec3(textureRect_.w, textureRect_.h, 0.0f), glm::vec2(normalize.w, normalize.h));      //glm::vec2(1.f,0.f)
        convex_[3] = UvVertex(glm::vec3(textureRect_.w, 0.f,            0.0f), glm::vec2(normalize.w, normalize.y));      //glm::vec2(1.f,1.f)
        
        needUpConvex_ = 0;
    }

    bool needUpConvex_ = 1;
    ConvexUV convex_;
    Color color_;
    FloatRect textureRect_;

    const Texture2D* texture_;
};

#endif