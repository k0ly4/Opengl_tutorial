
#ifndef GLYPH_H
#define GLYPH_H

#include "Resource/FontLoader.h" 
#include "Graphic/Texture.h"

/////Glyph----------------------------------------------------
class Glyph
{
public:

    FloatRect rect;
    glm::ivec2 bearing;
    glm::uvec2 size;
    size_t advantage;

    void load(const FT_Face& face);

    void setRect(const glm::vec2& position,const glm::ivec2& sizeTexture) {
        rect = FloatRect(position / glm::vec2(sizeTexture), glm::vec2(size) / glm::vec2(sizeTexture));
    }

    inline void useTexture()const {
        texture_->use(0);
    }

    void dellTexture() {
        texture_ = 0;
    }

private:

    std::shared_ptr<GeneralTexture2D> texture_ =0 ;

};

/////MapGlyph----------------------------------------------------
class MapGlyph {

public:
  
    MapGlyph() {}

    MapGlyph(Texture2D& gl_texture,const std::map<size_t, Glyph>& Alphabet) :
        texture_(gl_texture), alphabet_(Alphabet) {}

    const Texture2D& getTexture()const  {
        return texture_;
    }

    std::map<size_t, Glyph>& getMetrics()const {
        return alphabet_;
    }
private:

    Texture2D texture_;
    mutable std::map<size_t, Glyph> alphabet_;

};
#endif
