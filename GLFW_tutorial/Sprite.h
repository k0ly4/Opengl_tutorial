
#ifndef SPRITE_H
#define SPRITE_H

#include"Texture.h"
#include"Render.h"
#include "PrimitiveEntity.h"
class Sprite :public Drawable, public Transformable {
    
public:
    Sprite() : color(1.f), model_uv(1.f), model(1.f)
    {
        id_obj = glShader::sprite;
        this->initRenderData();
    }

    inline void setColor(const Color& color) {
        this->color = color;
    }
    void setTexture(Texture2D& texture);
    void setTextureRect(const FloatRect& cut_rect);

    void draw(View* view, Shader& shader);

private:

    static bool VAO_init;
    static ArrayBufferObject VAO;

    glm::vec2 size;
    Color color;
    glm::mat4 model, model_uv; 
    Texture2D texture;

    static void initRenderData();
    void transform();
};

#endif