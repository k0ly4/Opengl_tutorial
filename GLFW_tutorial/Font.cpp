#include "Font.h"

void Font::drawGlyph(Glyph& glyph, glm::vec2 pos) {
    glBindVertexArray(VAO);

    vertices[0][0] = pos.x;                 vertices[0][1] = pos.y + glyph.size.y;
    vertices[1][0] = pos.x + glyph.size.x;  vertices[1][1] = vertices[0][1];
    vertices[2][0] = pos.x;                 vertices[2][1] = pos.y;
    vertices[3][0] = vertices[1][0];        vertices[3][1] = pos.y;

    glyph.useTexture();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glyph.dellTexture();
}

bool Font::loadGlyph(Glyph& glyph, size_t ch) {
    if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
        std::cout << "(!)ERROR::Failed to load glyph: " << char(ch) << std::endl;
        return 0;
    }
    glyph.load(face);

    return 1;
}

void Font::display(std::map< size_t, Glyph>& alphabet, glm::uvec2 size, size_t step) {
    glm::vec2 pos(0.f);
    size_t count = 1;
    for (size_t i = MIN_CHAR_UTF; i < MAX_CHAR_UTF; i++, count++) {
        //  if (i == 42)continue;
        alphabet[i].uv = pos;
        drawGlyph(alphabet[i], pos);

        if (count % step == 0) {
            pos.y += size.y;
            pos.x = 0.f;
        }
        else pos.x += size.x;
    }
    for (size_t i = MIN_CHAR_UNI; i < MAX_CHAR_UNI; i++, count++) {
        alphabet[i].uv = pos;
        drawGlyph(alphabet[i], pos);

        if (count % step == 0) {
            pos.y += size.y;
            pos.x = 0.f;
        }
        else pos.x += size.x;

    }
}

MapGlyph Font::createFontTexture(size_t size_char) {

    FT_Set_Pixel_Sizes(face, 0, size_char);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glm::uvec2 max_size(0.f);
    std::map< size_t, Glyph> alphabet;
    for (size_t i = MIN_CHAR_UTF; i < MAX_CHAR_UTF; i++) {
        // if (i == 42)continue;
        Glyph glyph;
        if (loadGlyph(glyph, i)) {

            if (max_size.x < glyph.size.x) max_size.x = glyph.size.x;
            if (max_size.y < glyph.size.y)max_size.y = glyph.size.y;
            alphabet[i] = glyph;
        }
    }
    for (size_t i = MIN_CHAR_UNI; i < MAX_CHAR_UNI; i++) {
        Glyph glyph;
        if (loadGlyph(glyph, i)) {

            if (max_size.x < glyph.size.x) max_size.x = glyph.size.x;
            if (max_size.y < glyph.size.y)max_size.y = glyph.size.y;
            alphabet[i] = glyph;
        }
    }

    size_t sym_y = sqrt(alphabet.size() * (float)max_size.x / max_size.y),
        sym_x = alphabet.size() / sym_y + 1;

    glm::uvec2 size_texture = {
        sym_x * max_size.x,
        sym_y * max_size.y
    };

    render.create(size_texture.x, size_texture.y, GL_RED, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);//GL_LINEAR
    GlRender::setClearColor(0.f);
    GlRender::bind(render);
    glClear(GL_COLOR_BUFFER_BIT);

    glShader::get(glShader::font).use();
    glShader::get(glShader::font).uniform("projection", glm::ortho(0.f, (float)size_texture.x, (float)size_texture.y, 0.f));
    display(alphabet, max_size, sym_x);

    GlRender::unbind();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    return MapGlyph(render.getTexture(), alphabet, size_char);
}

Font::Font() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertices[0][2] = 0.0f;              vertices[0][3] = 1.f;
    vertices[1][2] = 1.0f;               vertices[1][3] = 1.0f;
    vertices[2][2] = 0.0f;              vertices[2][3] = 0.0f;
    vertices[3][2] = 1.f;               vertices[3][3] = 0.f;

    if (FT_Init_FreeType(&ft))
        std::cout << "(!)ERROR::FREETYPE:: Could not init freetype lib." << std::endl;
}

MapGlyph& Font::getTexture(size_t size_char) {
    for (auto iter = lib_glyph.begin(); iter != lib_glyph.end(); iter++)
        if (*iter == size_char)
            return *iter;
    std::cout << "create mapfont:" << size_char << std::endl;
    lib_glyph.push_back(createFontTexture(size_char));
    return lib_glyph.back();
}

bool Font::loadFromFile(const std::string& path) {
    if (FT_New_Face(ft, path.c_str(), 0, &face)) {
        std::cout << "(!)ERROR:: Failed to load font from " << path << std::endl;
        return 0;
    }
    return 1;
}
Font::~Font() {
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}