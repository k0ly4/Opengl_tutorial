#ifndef FONT_H
#define FONT_H

#include "Resource/Glyph.h"
#include "Graphic/Framebuffer.h"
#include "Scene/Convex.h"

class Font {

public:

    Font();

    MapGlyph& getMapGlyphs(size_t size_char);
    bool load(const std::string& path);

private:

    std::string path_;
    unsigned alphCode_[288];
    size_t alphSize;

    RenderTexture renderTexture_;
    std::map<size_t, MapGlyph> mapGlyphs;
    std::shared_ptr< FaceFont> face_;
    ConvexUV convex;

    void fillUnicode();
    bool loadGlyph(Glyph& glyph, size_t ch);
    void render(const glm::ivec2& sizeRender, std::map< size_t, Glyph>& alphabet, const glm::uvec2& size,  size_t step);
    MapGlyph createFontMap(size_t size_char);
    void drawGlyph(Glyph& glyph, const glm::vec2& pos);
};
#endif


