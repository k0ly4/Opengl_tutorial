
#ifndef TEXT_H
#define TEXT_H

#include"Font.h"

class Text :public Drawable, public Transformable2D {
   
public:
    Text() : 
        convex_(4),
        color_(1.f) 
    {
        id_obj = glShader::text;
        this->initRenderData();
    }

    void setFont(Font& font);

    void setSizeFont(size_t size);

    inline void setColor(const Color& color) {
        color_ = color;
    }
   
    void setString(const std::wstring& text);

    inline const std::wstring& getString()const {
        return strUnicode_;
    }
    void draw(const View* view, const Shader& shader);

private:

    Font* font_;
    const MapGlyph* map_ = 0;
    float originLineY_;
    bool needUpBuffer_ = 1;

    VertexBufferObject  transformsBuffer,uvBuffer;
    size_t sizeElementBuffer;

    size_t fontSize_;
    Color color_;
    std::wstring strUnicode_;
    ConvexUV convex_;

    void initRenderData();
    glm::mat4 getTranform(Glyph& glyph, const glm::vec2& pos);

    inline glm::vec2 getPos(Glyph& glyph, const glm::vec2& origin) {
        return origin + glm::vec2(glyph.bearing.x, originLineY_ - glyph.bearing.y) * getScale();
    }
    void upBuffer();

};
#endif
