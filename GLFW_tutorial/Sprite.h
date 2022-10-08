
#ifndef SPRITE_H
#define SPRITE_H

#include"Texture.h"
#include"Render.h"
#include "PrimitiveEntity.h"
#include "Convex.h"

///Sprite----------------------------------------------
/// <summary>
/// 
/// </summary>
class Sprite :public Texturable, public Transformable2D {
    
public:

    Sprite() :
        convex_(4),
        color_(1.f), 
        textureRect_(0.f)
    {}

    Sprite(const Texture2D& texture) :
        Sprite()
    {
        setTexture(texture);
    }

    void setTexture(const Texture2D& texture);
    void setTextureRect(const FloatRect& cut_rect);
    const FloatRect& getTextureRect() const {
        return textureRect_;
    }
    void draw(const View* view,const Shader& shader);

private:

    void upConvex();

    bool needUpConvex_ = 1;
    ConvexUV convex_;
    Color color_;
    FloatRect textureRect_;
};

#endif