#ifndef SPRITE_H
#define SPRITE_H

#include "Graphic/Texture.h"
#include "Graphic/Drawable.h"
#include "Graphic/Geometry.h"
#include "Resource/ShaderArguments.h"
#include "Math/Transform.h"

///Sprite----------------------------------------------
/// <summary>
/// 
/// </summary>
class Sprite :public Drawable {
    
public:

    Sprite() :
        color_(1.f), 
        textureRect_(0.f),
        texture(0)
    {
        shaderHint = glShader::texture_2d;
        mesh.vertices.resize(4);
    }

    Sprite(const Texture2D& texture) : Sprite()
    {
        setTexture(texture);
    }

    void setTexture(const Texture2D& texture);
    void setTextureRect(const FloatRect& cut_rect);
    const FloatRect& getTextureRect() const {return textureRect_; }
    const Texture2D* getTexture()const { return texture; }
    void draw(const View* view,const Shader& shader);

    void setColor(const Color& color) { color_ = color; }
    const Color& getColor()const { return color_; }
   
   
    Transform2D transform;

private:
    const Texture2D* texture;
    void solve();

    qGeometry<Vertex1<glm::vec4>> mesh;
    bool modified = 1;
    Color color_;
    FloatRect textureRect_;
};

#endif