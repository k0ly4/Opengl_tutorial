
#ifndef GLYPH_H
#define GLYPH_H

#include <ft2build.h>
#include FT_FREETYPE_H  
#include "Texture.h"

class Glyph
{
public:
    unsigned int texture;
    glm::vec2 uv;
    glm::uvec2 bearing;
    glm::uvec2 size;
    size_t advantage;

    void load(FT_Face& face);

    inline void useTexture() {
        glTexture::active(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    inline void dellTexture() {
        glDeleteTextures(1, &texture);
    }
};

class MapGlyph {

public:
   
    inline bool operator ==(size_t Font_Size) {
        return font_size == Font_Size;
    }

    MapGlyph() {}

    MapGlyph(Texture2D& gl_texture, std::map< size_t, Glyph>& Alphabet, size_t gl_font_size) :
        texture(gl_texture), alphabet(Alphabet), font_size(gl_font_size) {}

    Texture2D texture;
    size_t font_size = 0;
    std::map< size_t, Glyph> alphabet;
};
#endif
