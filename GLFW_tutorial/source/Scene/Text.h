
#ifndef TEXT_CLASS_H
#define TEXT_CLASS_H

#include    "Math/Transform.h"
#include    "Resource/Font.h"
#include    "Graphic/Geometry.h"
#include    "Input/Keyboard.h"

///FontMapPtr------------------------------------------------
/// <summary>
/// 
/// </summary>
class FontMapPtr {
public:
    FontMapPtr() :
        sizeFont_(20) {}
    inline bool isEmpty()const { return font_ == 0; }

    inline void setFont(Font& font) {
        if (font_ == &font) return;
        font_ = &font;
        modified = 1;
    }
    inline void setSizeFont(size_t size) {
        if (sizeFont_ == size)return;
        sizeFont_ = size;
        modified = 1;
    }

    inline size_t originLine() {                       return get()->metrics['h'].bearing.y;}

    inline const MapGlyph& operator*() {               return *get(); }
    inline MapGlyph* operator->() {                    return get(); }
    inline const MapGlyph* operator->() const{         return get(); }
    inline size_t sizeFont()const {                    return sizeFont_; }

private:

    inline MapGlyph* get() {
        if (modified) {
            modified = 0;
            map_ = &font_->getMapGlyphs(sizeFont_);
        }
        return map_;
    }
    inline const MapGlyph* get()const noexcept{ return get();}

    size_t sizeFont_;
    mutable bool modified = 0;
    Font* font_ = 0;
    mutable MapGlyph* map_ = 0;
};

///StyleText------------------------------------------------
/// <summary>
/// 
/// </summary>
struct StyleTextUniform {

       float softness;
       float thickness;
       Color color;

   StyleTextUniform():
        softness(0.f),
        thickness(0.5f),
        color(Color::WHITE)
    {}

   inline void uniform(const std::string& name, const Shader& shader) {
        shader.uniform(name + ".thickness", thickness);
        shader.uniform(name + ".softness",  softness);
        shader.uniform(name + ".color",     color);
    }
};

///CharMetrica------------------------------------------------
/// <summary>
/// 
/// </summary>
struct CharMetrica {
    glm::vec4 rect;
    glm::vec2 size;
    glm::mat4 model;
};

///Text------------------------------------------------
/// <summary>
/// 
/// </summary>
class Text :public Drawable{

public:

    Text():
        strUnicode(L"Коля лучший")
    {
        shaderHint = glShader::text;
        initRenderData();
    }

    void setFont(Font& font) {
        map_.setFont(font);
        modified = 1;
    }

    void setSizeFont(size_t size) {
        map_.setSizeFont(size);
        modified = 1;
    }

    inline void setString(const std::wstring& text) {
        strUnicode = text;
        modified = 1;
    }
    void draw(const View* view, const Shader& shader);

    Transform2D transform;

    StyleTextUniform style;
    StyleTextUniform outline;

private:

    glm::vec4 getGlyphRect(const Glyph& glyph)const ;
    //возвращает кол-во символов без учета '\n'
    inline static size_t getSize(const std::wstring& str) {
        size_t result = 0;
        for (size_t i = 0; i < str.size(); i++) {
            if (str[i] == '\n')continue;
            result++;
        }
        return result;
    }

    void solve();

    inline glm::vec2 getLocalPos(const Glyph& glyph,float originLineY){
        return glm::vec2(glyph.bearing.x, originLineY - glyph.bearing.y);
    }

    inline void upBufferMetrica() {
        bufferMetrica.begin();
        bufferMetrica.data(charBuffer);
    }

   
    std::wstring strUnicode;
    
    bool modified = 0;
    FontMapPtr map_;

    VertexBufferObject bufferMetrica;
    std::vector<CharMetrica> charBuffer;
    qGeometry<Vertex2<glm::vec3, glm::vec2>>mesh; 
    void initRenderData();
};
#endif
