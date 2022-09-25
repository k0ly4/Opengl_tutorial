#ifndef FONT_H
#define FONT_H

#include "Glyph.h"
#include "Framebuffer.h"

class Font {

public:

    Font();

    MapGlyph& getTexture(size_t size_char);
    bool loadFromFile(const std::string& path);

    ~Font();

private:

    FT_Library ft;
    FT_Face face;

    const size_t MAX_CHAR_UTF = 256;
    const size_t MIN_CHAR_UTF = 32;

    const size_t MAX_CHAR_UNI = 1104;
    const size_t MIN_CHAR_UNI = 1040;

    float vertices[4][4];
    unsigned int VAO,VBO;

    RenderTexture render;
    std::list<MapGlyph> lib_glyph;

    bool loadGlyph(Glyph& glyph, size_t ch);
    void display(std::map< size_t, Glyph>& alphabet, glm::uvec2 size, size_t step);
    MapGlyph createFontTexture(size_t size_char);
    void drawGlyph(Glyph& glyph, glm::vec2 pos);
};
#endif


