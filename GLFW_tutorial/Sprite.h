
#ifndef SPRITE_H
#define SPRITE_H

#include"Texture.h"
#include"Render.h"

class Sprite :public Drawable, public Transformable {
    Texture2D texture;

    static bool VAO_init;
    static ArrayBufferObject VAO;

    glm::vec2 size;
    Color color;
    glm::mat4 model, model_uv;

 static void initRenderData()
    {
     if (VAO_init == 0) {
         Primitive::get(Primitive::quad2d).begin();
         VAO.begin();
         VAO.attrib(0, 4, 4 * sizeof(float), 0);
         VAO_init = 1;
     }
    }
    void transform() {
        glm::vec2 size = scale * this->size;
        model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(origin + position, 0.0f));
        model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::translate(model, glm::vec3(- scale * origin, 0.0f));

        model = glm::scale(model, glm::vec3(size, 1.0f)); // последним идет масштабирование
    }
public:
    Sprite() : color(1.f), model_uv(1.f), model(1.f)
    {
        id_obj = glShader::sprite;
        this->initRenderData();
    }
    inline void setColor(const Color& color) {
        this->color = color;
    }
    inline void setTexture(Texture2D& texture) {
        this->texture = texture;
        size = texture.getSize();
        need_update_model = 1;
    }
    void setTextureRect(const FloatRect& cut_rect) {

        model_uv = glm::translate(glm::mat4(1.f),
            glm::vec3(cut_rect.x / texture.getSize().x, (texture.getSize().y - cut_rect.h - cut_rect.y) / texture.getSize().y, 0.f));
        model_uv = glm::scale(model_uv,
            glm::vec3(cut_rect.w / texture.getSize().x, cut_rect.h / texture.getSize().y, 0.f));

        size.x = cut_rect.w;
        size.y = cut_rect.h;
        need_update_model = 1;
    }
    void draw(View* view,Shader& shader) {
        shader.use();
        view->use(shader);
        shader.uniform("color", color);
        shader.uniform("model_uv", model_uv);
        if (need_update_model) {
            transform();
            need_update_model = 0;
        }
        shader.uniform("model", model);
        texture.use(0);
        VAO.begin();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        VAO.end();
    }
};
ArrayBufferObject Sprite::VAO;
bool Sprite::VAO_init = 0;
#endif