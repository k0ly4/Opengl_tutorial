#include "Text.h"

void Text::initRenderData()
{
    convex_[0] = UvVertex(glm::vec3(0.0f, 1.0f, 0.f), glm::vec2(0.f, 1.f));
    convex_[1] = UvVertex(glm::vec3(0.0f, 0.0f, 0.f), glm::vec2(0.f, 0.f));
    convex_[2] = UvVertex(glm::vec3(1.0f, 1.0f, 0.f), glm::vec2(1.f, 1.f));
    convex_[3] = UvVertex(glm::vec3(1.0f, 0.0f, 0.f), glm::vec2(1.f, 0.f));

    bufferMetrica.setMode(GBO::DYNAMIC);
    auto& VAO = convex_.getVAO();
    VAO.data_draw.cur_type = DataDraw::DrawArraysInstanced;
    VAO.begin();
    //metrics
    bufferMetrica.begin();

    VAO.attribInstance(2, 4,  sizeof(CharMetrica), 0);
    VAO.attribInstance(3, 2,  sizeof(CharMetrica), sizeof(glm::vec4));
    VAO.attribInstanceMat4(4, sizeof(CharMetrica), sizeof(glm::vec4)+sizeof(glm::vec2));
}

void Text::draw(const View* view, const Shader& shader) {
    if (needUpMap ) 
        upMap();
  
    shader.use();
    view->use(shader);
    uniformTransform(shader);
    form_.uniform("style", shader);
    outline_.uniform("outline", shader);
    map_.getTexture().use(0);
    convex_.draw();
}

glm::vec4 Text::getGlyphRect(const Glyph& glyph)const {
    FloatRect rect = glyph.rect;
    rect.w = rect.x + rect.w;
    //flip vertically
    rect.y = 1.f - rect.y;
    rect.h = rect.y - rect.h;
    return rect.vec4();
}

size_t Text::getSize(const std::wstring& str) {
    size_t result = 0;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '\n')continue;
        result++;
    }
    return result;
}

void Text::upMap() {

    charData_.resize(getSize(strUnicode));
    convex_.getVAO().data_draw.data.count_object = charData_.size();

    glm::vec2 pos(0.f);
    size_t index = 0;

    std::map<size_t, Glyph>& glyphs = map_.getGlyphs();
    size_t originLineY = map_.getOriginLine();

    for (size_t i = 0; i < strUnicode.size(); i++) {

        if (strUnicode[i] == '\n') {
            //next line
            pos.y += (float)map_.getSizeFont();
            pos.x = 0.f;
            continue;
        }

        Glyph& glyph = glyphs[strUnicode[i]];

        charData_[index].size = glyph.size;
        charData_[index].rect = getGlyphRect(glyph);
        charData_[index].model = glm::translate(glm::mat4(1.f), glm::vec3(pos + getLocalPos(glyph, (float)originLineY), 0.f));
        //to right
        pos.x += (float)glyph.advantage;
        //step charData_
        index++;
    }

    upBuffer();
    needUpMap = 0;
}
