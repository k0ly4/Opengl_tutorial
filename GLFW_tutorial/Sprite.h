
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
        convex_[0] = UvVertex(glm::vec3(0.f,            textureRect_.h, 0.0f), glm::vec2(normalize.x, normalize.y));
        convex_[1] = UvVertex(glm::vec3(0.f,            0.f,            0.0f), glm::vec2(normalize.x, normalize.h));
        convex_[2] = UvVertex(glm::vec3(textureRect_.w, textureRect_.h, 0.0f), glm::vec2(normalize.w, normalize.y));
        convex_[3] = UvVertex(glm::vec3(textureRect_.w, 0.f,            0.0f), glm::vec2(normalize.w, normalize.h));

        needUpConvex_ = 0;
    }

    bool needUpConvex_ = 1;
    ConvexUV convex_;
    Color color_;
    FloatRect textureRect_;

    const Texture2D* texture_;
};

#endif