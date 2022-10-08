
#ifndef TEXT_H
#define TEXT_H

#include"Font.h"
#include "Keyboard.h"

///PointerMap------------------------------------------------
/// <summary>
/// 
/// </summary>
class PointerMap {
public:

    void setFont(Font& font) {
        if (font_ == &font) return;
        font_ = &font;
        needUpMap = 1;
    }

    void setSizeFont(size_t size) {
        if (sizeFont == size)return;
        sizeFont = size;
        needUpMap = 1;
    }

    bool isInitFont()const {
        return font_ != 0;
    }

    const Texture2D& getTexture()const {
        if (needUpMap) upMap();
        return map_->getTexture();
    }
    std::map<size_t, Glyph>& getGlyphs()const {
        if (needUpMap) upMap();
        return map_->getMetrics();
    }

    size_t getOriginLine() {
        if (needUpMap) upMap();
        return map_->getMetrics()['h'].bearing.y;
    }
    
    const MapGlyph& operator*()const {
        if (needUpMap) upMap();
        return *map_;
    }

    size_t getSizeFont()const {
        return sizeFont;
    }

private:

    void upMap()const {
        map_ = &font_->getMapGlyphs(sizeFont);
        needUpMap = 0;
    }


    size_t sizeFont = 20;

    mutable bool needUpMap = 0;
    Font* font_ = 0;
    mutable MapGlyph* map_ = 0;

};

///StyleText------------------------------------------------
/// <summary>
/// 
/// </summary>
struct StyleTextUniform {

    float softness_ = 0.f;
    float thickness_ = 0.5f;
    Color color_ = Color::GREEN;

    void uniform(const std::string& name, const Shader& shader) {
        shader.uniform(name + ".thickness", thickness_);
        shader.uniform(name + ".softness", softness_);
        shader.uniform(name + ".color", color_);
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
class Text :public Drawable, public Transformable2D {

public:

    Text() :
        convex_(4)
    {
        shaderHint = glShader::text;

        initRenderData();
    }

    void setFont(Font& font) {
        map_.setFont(font);
        needUpMap = 1;
    }

    void setSizeFont(size_t size) {
        map_.setSizeFont(size);
        needUpMap = 1;
    }

    inline void setColor(const Color& color) {
        form_.color_ = color;
    }

    const Color& getColor()const {
        return form_.color_;
    }

    void setString(const std::wstring& text) {
        strUnicode = text;
        needUpMap = 1;
    }

    void input(size_t unicode,bool isKeyEdit) {
        if (isKeyEdit) {
            if (unicode == Keyboard::Backspace) {
                strUnicode.pop_back();
            }
            else  if (unicode == Keyboard::Enter) {
                strUnicode += '\n';
            }
        }
        else {
            strUnicode.push_back((wchar_t)unicode);
        }
        needUpMap = 1;
    }

    StyleTextUniform& getFormEffects() {
        return form_;
    }

    StyleTextUniform& getOutlineEffects() {
        return outline_;
    }

    void draw(const View* view, const Shader& shader);

private:

    glm::vec4 getGlyphRect(const Glyph& glyph)const ;

    size_t getSize(const std::wstring& str);

    void upMap();

    inline glm::vec2 getLocalPos(const Glyph& glyph,float originLineY){
        return glm::vec2(glyph.bearing.x, originLineY - glyph.bearing.y);
    }

    void upBuffer() {
        //metrics
        bufferMetrica.begin();
        bufferMetrica.data(charData_);
   
    }

    std::vector<CharMetrica> charData_;
  
    VertexBufferObject bufferMetrica;
    bool needUpMap = 0;
    PointerMap map_;

    std::wstring strUnicode;
    ConvexUV convex_;

    StyleTextUniform form_;
    StyleTextUniform outline_;

    void initRenderData();
};
#endif
