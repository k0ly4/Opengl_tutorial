#include "Text.h"

    void Text::initRenderData()
    {
        convex_[0] = UvVertex(glm::vec3( 0.0f, 0.0f,0.f), glm::vec2(0.0f, 1.f));
        convex_[1] = UvVertex(glm::vec3( 1.0f, 0.0f,0.f), glm::vec2(1.f, 1.f));
        convex_[2] = UvVertex(glm::vec3( 0.0f, 1.0f,0.f), glm::vec2(0.f, 0.f));
        convex_[3] = UvVertex(glm::vec3( 1.0f, 1.0f,0.f), glm::vec2(1.f, 0.f));
    }

    glm::mat4 Text::getTranform(Glyph& glyph, const glm::vec2& pos) {
        glm::vec2 size = getScale() * glm::vec2(glyph.size);
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(getOrigin() + getPosition(), 0.0f));
        model = glm::rotate(model, getRotate(), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::translate(model, glm::vec3(pos - getScale() * getOrigin() - getPosition(), 0.0f));

        model = glm::scale(model, glm::vec3(size, 1.0f)); // последним идет масштабирование
       
        return model;
    }

    void Text::upBuffer() {
        //Create data
        sizeElementBuffer = strUnicode_.size();

        std::vector<glm::mat4> transforms(sizeElementBuffer);
        std::vector<glm::vec4> uvData(sizeElementBuffer);
        glm::vec2 pos = getPosition();

        Glyph glyph;
      
        for (size_t i = 0; i < sizeElementBuffer; i++) {

            if (strUnicode_[i] == '\n') {
                pos.x = getPosition().x;
                pos.y += fontSize_ * getScale().y;
                continue;
            }

            glyph = map_->getMetrics()[strUnicode_[i]];
          
            transforms[i] = getTranform(glyph, getPos(glyph, pos));
            uvData[i] = glyph.rect.vec4();

            pos.x += glyph.advantage * getScale().x;
        } 

        ///Buffer load data
        ArrayBufferObject& VAO = convex_.getVAO();
       
        VAO.begin();
        uvBuffer.begin();

        uvBuffer.setMode(GL_DYNAMIC_DRAW);
        uvBuffer.data(uvData);
        VAO.attribInstance(2, 4, sizeof(float) * 4, 0);
        
        transformsBuffer.begin();

        transformsBuffer.setMode(GL_DYNAMIC_DRAW);
        transformsBuffer.data(transforms);
        VAO.attribInstanceVBOMat4(3);

        transformsBuffer.end();
        VAO.end();
    }

    void Text::setFont(Font& font) {
        font_ = &font;
        needUpBuffer_ = 1;
    }

    void Text::setSizeFont(size_t size) {
        if (fontSize_ == size)return;

        fontSize_ = size;
        map_ = &font_->getMapGlyphs(fontSize_);
        originLineY_ = map_->getMetrics()['h'].bearing.y;
        needUpBuffer_ = 1;
    }

    //Определения виртуальных transform2d функций
    void Text::setString(const std::wstring& text) {
        strUnicode_ = text;
        needUpBuffer_ = 1;
    }
   
    void Text::draw(const View* view, const Shader& shader) {
        
        if (needUpBuffer_||transform_.isNeedUp()) {
            upBuffer();
            needUpBuffer_ = 0;
        }

        shader.use();
        view->use(shader);ssssssss
        uniformTransform(shader);
        shader.uniform("color", color_);
        map_->getTexture().use(0);

        convex_.getVAO().data_draw.data.count_object = sizeElementBuffer;
        convex_.draw();
    }

