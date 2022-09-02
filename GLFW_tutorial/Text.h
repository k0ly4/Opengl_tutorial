#pragma once
#ifndef TEXT_H
#define TEXT_H

#include "Framebuffer.h"
#include <ft2build.h>
#include FT_FREETYPE_H  

struct Glyph
{

    unsigned int texture;
    glm::vec2 uv;
    glm::uvec2 bearing;
    glm::uvec2 size;
    size_t advantage;

    void load(FT_Face& face) {
        size = glm::uvec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        bearing = glm::uvec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        advantage = face->glyph->advance.x >> 6;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    }
    inline void useTexture() {
        glTexture::active(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    inline void dellTexture() {
        glDeleteTextures(1, &texture);
    }
};
struct Glyphs {
    Texture2D texture;
    size_t font_size = 0;
    std::map< size_t, Glyph> alphabet;
    inline bool operator ==(size_t Font_Size) {
        return font_size == Font_Size;
    }
    Glyphs() {}
    Glyphs(Texture2D& gl_texture, std::map< size_t, Glyph>& Alphabet, size_t gl_font_size) :
        texture(gl_texture), alphabet(Alphabet), font_size(gl_font_size) {}

};
class Font {
    const size_t MAX_CHAR_UTF = 256,
        MIN_CHAR_UTF = 32;
    const size_t MAX_CHAR_UNI = 1104,
        MIN_CHAR_UNI = 1040;
    float vertices[4][4];
    unsigned VAO, VBO;

    void drawGlyph(Glyph& glyph, glm::vec2 pos) {
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
    RenderTexture render;
    FT_Library ft;
    FT_Face face;
    std::list<Glyphs> lib_glyph;
    bool loadGlyph(Glyph& glyph, size_t ch) {
        if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
            std::cout << "(!)ERROR::Failed to load glyph: " << char(ch) << std::endl;
            return 0;
        }
        glyph.load(face);

        return 1;
    }
    void display(std::map< size_t, Glyph>& alphabet, glm::uvec2 size, size_t step) {
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
    Glyphs createFontTexture(size_t size_char) {

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

        return Glyphs(render.getTexture(), alphabet, size_char);
    }
public:
    Font() {
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
    Glyphs& getTexture(size_t size_char) {
        for (auto iter = lib_glyph.begin(); iter != lib_glyph.end(); iter++)
            if (*iter == size_char)
                return *iter;
        std::cout << "create mapfont:" << size_char << std::endl;
        lib_glyph.push_back(createFontTexture(size_char));
        return lib_glyph.back();
    }
    bool loadFromFile(const std::string& path) {
        if (FT_New_Face(ft, path.c_str(), 0, &face)) {
            std::cout << "(!)ERROR:: Failed to load font from " << path << std::endl;
            return 0;
        }
        return 1;
    }
    ~Font() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
};
class Text :public Drawable, public Transformable2D {
    Font* font;
    Glyphs* gl = 0;
    float origin_line_y;
    bool needUpBuffer = 1;

    VertexBufferObject VBO, m_buffer;
    ArrayBufferObject VAO;
    size_t size_element_render;
    const float vertices[16] = {
          0.0f, 0.0f, 0.0f, 1.0f,
          1.0f, 0.0f, 1.0f, 1.0f,
          0.0f, 1.0f, 0.0f, 0.0f,
          1.0f, 1.0f, 1.0f, 0.0f
    };

    Color color;
    std::wstring str;

    void initRenderData()
    {
        VBO.begin();
        VBO.data(sizeof(vertices), vertices);

        VAO.begin();
        VAO.attrib(0, 4, 4 * sizeof(float), 0);

        VBO.end();
        VAO.end();
    }
    inline FloatRect normalize(const FloatRect& r) {
        return FloatRect(r.x / gl->texture.getSize().x, r.y / gl->texture.getSize().y, r.w / gl->texture.getSize().x, r.h / gl->texture.getSize().y);
    }
    glm::mat4 transform(Glyph& glyph, glm::vec2 pos) {
        glm::vec2 size = scale * glm::vec2(glyph.size);
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(origin+ position, 0.0f));
        model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
        
        model = glm::translate(model, glm::vec3(pos - scale * origin - position, 0.0f));
       
        model = glm::scale(model, glm::vec3(size, 1.0f)); // последним идет масштабирование
        return model;
    }
    inline glm::vec2 getPos(Glyph& glyph, glm::vec2 origin) {
        return origin +glm::vec2(glyph.bearing.x, origin_line_y- glyph.bearing.y)* scale;
    }
    glm::mat4 transform_uv(Glyph& glyph) {

        FloatRect cut_rect(glyph.uv, glyph.size);
        cut_rect = normalize(cut_rect);

        glm::mat4 model = glm::translate(glm::mat4(1.f),
            glm::vec3(cut_rect.x, 1.f - cut_rect.h - cut_rect.y, 0.f));
        model = glm::scale(model,
            glm::vec3(cut_rect.w, cut_rect.h, 0.f));

        return model;
    }
    void upBuffer() {
        std::vector<glm::mat4> mat;
        mat.reserve(2 * str.size());

        glm::vec2 pos = position;

        Glyph glyph;
        size_element_render = 0;
        for (size_t i = 0 ; i < str.size(); i++) {
            if (str[i] == '\n') {
                pos.x = position.x;
                pos.y += gl->font_size* scale.y;
            }
            else {
                glyph = gl->alphabet[str[i]];
                mat.push_back(transform(glyph, getPos(glyph, pos)));
                mat.push_back(transform_uv(glyph));
                pos.x += glyph.advantage * scale.x;
                size_element_render++;
            }
        }
       
        m_buffer.begin();
        m_buffer.setMode(GL_DYNAMIC_DRAW);
        m_buffer.data(mat.size() * sizeof(glm::mat4), mat.data());

        VAO.begin();

        size_t size4 = sizeof(float) * 4;
        size_t step = size4 * 8;
        for (size_t i = 1; i < 9; i++) {

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, step, (void*)(size4 * (i - 1)));
            glVertexAttribDivisor(i, 1);
        }

        m_buffer.end();
        VAO.end();
    }
public:
    Text() : color(1.f) 
    {
        id_obj = glShader::text;
        this->initRenderData();
    }
    inline void setFont(Font& font) {
        this->font = &font;
        needUpBuffer = 1;
    }
    inline void setSizeFont(size_t size) {
        gl = &font->getTexture(size);
        origin_line_y = gl->alphabet['h'].bearing.y;
        needUpBuffer = 1;
    }
    inline void setColor(const Color& color) {
        this->color = color;
    }
    //Определения виртуальных transform2d функций
    inline void setPosition(glm::vec2 position) {
        this->position = position;
        needUpBuffer = 1;
    }
    inline void setRotate(float angle) {
        rotate = angle;
        needUpBuffer = 1;
    }
    inline void setOrigin(glm::vec2 origin) {
        this->origin = origin;
        needUpBuffer = 1;
    }
    inline void setScale(glm::vec2 scale) {
        this->scale = scale;
        needUpBuffer = 1;
    }

    void setPosition(float x, float y) {
        setPosition(glm::vec2(x, y));
    }
    void setScale(float x, float y) {
        setScale(glm::vec2(x, y));
    }
    void setOrigin(float x, float y) {
        setOrigin(glm::vec2(x, y));
    }

    inline void setString(const std::wstring& text) {
        str = text;
        needUpBuffer = 1;
    }
    inline const std::wstring& getString() {
        return str;
    }
    void draw(Shader& shader) {
        if (needUpBuffer) {
            upBuffer();
            needUpBuffer = 0;
        }
        shader.use();
        shader.uniform("color", color);   
        gl->texture.use(0);
        VAO.begin();
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, size_element_render);
    }
};
#endif