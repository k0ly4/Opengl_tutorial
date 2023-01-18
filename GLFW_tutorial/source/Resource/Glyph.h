
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

    inline void useTexture()const noexcept { texture_->use(0);  }
    inline void dellTexture()noexcept { texture_ = 0; }

private:

    std::shared_ptr<Texture2DLite> texture_ =0 ;

};
typedef std::map<size_t, Glyph> Glyphs;
/////MapGlyph----------------------------------------------------
class MapGlyph {

public:
    MapGlyph() {}

    MapGlyph(Texture2D& gl_texture,const Glyphs& Alphabet):texture_(gl_texture), metrics(Alphabet) {}

    const Texture2D& getTexture()const  { return texture_; }
    Glyphs metrics;

private:
    Texture2D texture_;
};
#endif
