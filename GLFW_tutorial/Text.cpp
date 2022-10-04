//#include "Text.h"
//
//    void Text::initRenderData()
//    {
//        VBO.begin();
//        VBO.data(sizeof(vertices), vertices);
//
//        VAO.begin();
//        VAO.attrib(0, 4, 4 * sizeof(float), 0);
//
//        VBO.end();
//        VAO.end();
//    }
//
//    glm::mat4 Text::transform(Glyph& glyph, glm::vec2 pos) {
//        glm::vec2 size = scale * glm::vec2(glyph.size);
//        glm::mat4 model = glm::mat4(1.0f);
//
//        model = glm::translate(model, glm::vec3(origin + position, 0.0f));
//        model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
//
//        model = glm::translate(model, glm::vec3(pos - scale * origin - position, 0.0f));
//
//        model = glm::scale(model, glm::vec3(size, 1.0f)); // последним идет масштабирование
//        return model;
//    }
//
//    glm::mat4 Text::transform_uv(Glyph& glyph) {
//
//        FloatRect cut_rect(glyph.uv, glyph.size);
//        cut_rect = normalize(cut_rect);
//
//        glm::mat4 model = glm::translate(glm::mat4(1.f),
//            glm::vec3(cut_rect.x, 1.f - cut_rect.h - cut_rect.y, 0.f));
//        model = glm::scale(model,
//            glm::vec3(cut_rect.w, cut_rect.h, 0.f));
//
//        return model;
//    }
//
//    void Text::upBuffer() {
//        std::vector<glm::mat4> mat;
//        mat.reserve(2 * str.size());
//
//        glm::vec2 pos = position;
//
//        Glyph glyph;
//        size_element_render = 0;
//        for (size_t i = 0; i < str.size(); i++) {
//            if (str[i] == '\n') {
//                pos.x = position.x;
//                pos.y += gl->font_size * scale.y;
//            }
//            else {
//                glyph = gl->alphabet[str[i]];
//                mat.push_back(transform(glyph, getPos(glyph, pos)));
//                mat.push_back(transform_uv(glyph));
//                pos.x += glyph.advantage * scale.x;
//                size_element_render++;
//            }
//        }
//
//        m_buffer.begin();
//        m_buffer.setMode(GL_DYNAMIC_DRAW);
//        m_buffer.data(mat.size() * sizeof(glm::mat4), mat.data());
//
//        VAO.begin();
//
//        size_t size4 = sizeof(float) * 4;
//        size_t step = size4 * 8;
//        for (size_t i = 1; i < 9; i++) {
//
//            glEnableVertexAttribArray(i);
//            glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, step, (void*)(size4 * (i - 1)));
//            glVertexAttribDivisor(i, 1);
//        }
//
//        m_buffer.end();
//        VAO.end();
//    }
//
//    void Text::setFont(Font& font) {
//        this->font = &font;
//        needUpBuffer = 1;
//    }
//
//    void Text::setSizeFont(size_t size) {
//        gl = &font->getTexture(size);
//        origin_line_y = gl->alphabet['h'].bearing.y;
//        needUpBuffer = 1;
//    }
//    //Определения виртуальных transform2d функций
//    void Text::setPosition(glm::vec2 position) {
//        this->position = position;
//        needUpBuffer = 1;
//    }
//
//    void Text::setRotate(float angle) {
//        rotate = angle;
//        needUpBuffer = 1;
//    }
//
//    void Text::setOrigin(glm::vec2 origin) {
//        this->origin = origin;
//        needUpBuffer = 1;
//    }
//
//    void Text::setScale(glm::vec2 scale) {
//        this->scale = scale;
//        needUpBuffer = 1;
//    }
//
//    void Text::setString(const std::wstring& text) {
//        str = text;
//        needUpBuffer = 1;
//    }
//   
//    void Text::draw(Shader& shader) {
//        if (needUpBuffer) {
//            upBuffer();
//            needUpBuffer = 0;
//        }
//        shader.use();
//        shader.uniform("color", color);
//        gl->texture.use(0);
//        VAO.begin();
//        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, size_element_render);
//    }

