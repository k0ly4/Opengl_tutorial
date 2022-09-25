
#ifndef TEXT_H
#define TEXT_H

#include"Font.h"

class Text :public Drawable, public Transformable2D {
   
public:
    Text() : color(1.f) 
    {
        id_obj = glShader::text;
        this->initRenderData();
    }
    void setFont(Font& font);
    void setSizeFont(size_t size);

    inline void setColor(const Color& color) {
        this->color = color;
    }
    //Определения виртуальных transform2d функций
    void setPosition(glm::vec2 position);
    void setRotate(float angle);
    void setOrigin(glm::vec2 origin);
    void setScale(glm::vec2 scale);

    void setPosition(float x, float y) {
        setPosition(glm::vec2(x, y));
    }
    void setScale(float x, float y) {
        setScale(glm::vec2(x, y));
    }
    void setOrigin(float x, float y) {
        setOrigin(glm::vec2(x, y));
    }

    void setString(const std::wstring& text);
    inline const std::wstring& getString() {
        return str;
    }
    void draw(Shader& shader);

private:

    Font* font;
    MapGlyph* gl = 0;
    float origin_line_y;
    bool needUpBuffer = 1;

    VertexBufferObject VBO, m_buffer;
    ArrayBufferObject VAO;
    size_t size_element_render;
    const float vertices[16] = {
          0.0f, 0.0f, 0.0f, 1.0f,
          1.0f, 0.0f, 1.0f, 1.0f,
          0.0f, 1.0f, 0.0f, 0.0f,
          1.0f, 1.0f, 1.0f, 0.0f
    };

    Color color;
    std::wstring str;

    void initRenderData();
    inline FloatRect normalize(const FloatRect& r) {
        return FloatRect(r.x / gl->texture.getSize().x, r.y / gl->texture.getSize().y, r.w / gl->texture.getSize().x, r.h / gl->texture.getSize().y);
    }
    glm::mat4 transform(Glyph& glyph, glm::vec2 pos);

    inline glm::vec2 getPos(Glyph& glyph, glm::vec2 origin) {
        return origin + glm::vec2(glyph.bearing.x, origin_line_y - glyph.bearing.y) * scale;
    }
    glm::mat4 transform_uv(Glyph& glyph);
    void upBuffer();

};
#endif
