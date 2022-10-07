#include "Font.h"

/// <summary>
/// Font
/// </summary>

Font::Font() :
    convex(4)
{
    convex[0].uv = glm::vec2(0.0f, 1.f);
    convex[1].uv = glm::vec2(1.f, 1.f);
    convex[2].uv = glm::vec2(0.f, 0.f);
    convex[3].uv = glm::vec2(1.f, 0.f);

    fillUnicode();
}

void Font::fillUnicode() {
    const size_t MAX_CHAR_UTF = 256;
    const size_t MIN_CHAR_UTF = 32;
    //224
    const size_t MAX_CHAR_UNI = 1104;
    const size_t MIN_CHAR_UNI = 1040;
    //64
    //Кириллица
    size_t index = 0;
    for (size_t i = MIN_CHAR_UTF; i < MAX_CHAR_UTF; i++) {
        alphCode_[index] = i;
        index++;
    }
    for (size_t i = MIN_CHAR_UNI; i < MAX_CHAR_UNI; i++) {
        alphCode_[index] = i;
        index++;
    }
    alphSize = index;
}

MapGlyph& Font::getMapGlyphs(size_t size_char) {
    auto map = mapGlyphs.find(size_char);
    if (map == mapGlyphs.end()) {
        std::cout << "Font::getTexture::Create mapfont:" << size_char << std::endl;
        mapGlyphs[size_char] = createFontMap(size_char);
        return mapGlyphs[size_char];
    }
    return map->second;
}

bool Font::load(const std::string& path) {
    if (path == path_) return 1;
    path_ = path;

    face_ = FontLoader::get(path);
    return face_ != 0;
}

MapGlyph Font::createFontMap(size_t size_char) {

    face_->setPixelSize(0, size_char);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glm::uvec2 max_size(0.f);
    std::map<size_t, Glyph> alphabet;

    for (size_t i = 0; i < alphSize; i++) {
        Glyph glyph;
        if (loadGlyph(glyph, alphCode_[i])) {

            if (max_size.x < glyph.size.x) max_size.x = glyph.size.x;
            if (max_size.y < glyph.size.y)max_size.y = glyph.size.y;
            alphabet[alphCode_[i]] = glyph;
        }
    }
    //Sym_y - высота в буквах
    //Sym_X - ширина в буквах
    size_t sym_y = sqrt(alphabet.size() * (float)max_size.x / (float)max_size.y),
        sym_x = alphabet.size() / sym_y + 1;
    //Max size render texture
    glm::ivec2 size_texture = {
        sym_x * max_size.x,
        sym_y * max_size.y
    };

    renderTexture_.create(size_texture, TextureData(GL_RED, GL_RED, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR));//GL_LINEAR
    GlRender::setClearColor(0.f, 0.f, 0.f, 0.f);
    GlRender::bind(renderTexture_);
    Depth::Test(false);
    CullFace::Enable(false);
    glClear(GL_COLOR_BUFFER_BIT);

    const Shader& shader = glShader::get(glShader::red_font);
    shader.use();
    shader.uniform("projection", glm::ortho(0.f, (float)size_texture.x, (float)size_texture.y, 0.f));
    render(size_texture, alphabet, max_size,  sym_x);    
    GlRender::unbind();
    Depth::Test(true);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    return MapGlyph(renderTexture_.getTexture(), alphabet);
}

bool Font::loadGlyph(Glyph& glyph, size_t ch) {
    if (face_->loadGlyph(ch, FT_LOAD_RENDER) == 0)
        return 0;
    FT_Render_Glyph(face_->getSlot(), FT_RENDER_MODE_SDF);
    glyph.load(face_->get());
    return 1;
}

void Font::render(const glm::ivec2& sizeRender, std::map< size_t, Glyph>& alphabet, const glm::uvec2& maxSizeGlyph, size_t step) {

    glm::vec2 pos(0.f);
    size_t count = 1;

    for (size_t i = 0; i < alphSize; i++, count++) {
        alphabet[alphCode_[i]].setRect(pos, sizeRender);
        drawGlyph(alphabet[alphCode_[i]], pos);

        if (count % step == 0) {
            pos.y += maxSizeGlyph.y;
            pos.x = 0.f;
        }
        else pos.x += maxSizeGlyph.x;
    }
}

void Font::drawGlyph(Glyph& glyph, const glm::vec2& pos) {

    convex[0].pos = glm::vec3(pos.x, pos.y + glyph.size.y, 0.f);
    convex[1].pos = glm::vec3(pos.x + glyph.size.x, pos.y + glyph.size.y, 0.f);
    convex[2].pos = glm::vec3(pos.x, pos.y, 0.f);
    convex[3].pos = glm::vec3(pos.x + glyph.size.x, pos.y, 0.f);

    glyph.useTexture();
    convex.getVAO().draw();
    glyph.dellTexture();
    //!need free resource glyph
   // glyph.dellTexture();
}


