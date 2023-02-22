#include "Text.h"

void Text::initRenderData()
{
    mesh.vertices.resize(4);

    mesh.vertices[0] = {glm::vec3(0.0f, 1.0f, 0.f), glm::vec2(0.f, 1.f)};
    mesh.vertices[1] = {glm::vec3(0.0f, 0.0f, 0.f), glm::vec2(0.f, 0.f)};
    mesh.vertices[2] = {glm::vec3(1.0f, 1.0f, 0.f), glm::vec2(1.f, 1.f)};
    mesh.vertices[3] = {glm::vec3(1.0f, 0.0f, 0.f), glm::vec2(1.f, 0.f)};
    mesh.saveInBuffer();

    bufferMetrica.setMode(GBO::DYNAMIC);
    mesh.VAO.begin();
    //metrics
    bufferMetrica.begin();

    mesh.VAO.attribInstance(2, 4,  sizeof(CharMetrica), 0);
    mesh.VAO.attribInstance(3, 2,  sizeof(CharMetrica), sizeof(glm::vec4));
    mesh.VAO.attribInstanceMat4(4, sizeof(CharMetrica), sizeof(glm::vec4)+sizeof(glm::vec2));
}

void Text::draw(const View* view, const Shader& shader) {
    if (modified) solve();
    shader.use();
    view->use(shader);
    shader.uniform("model", transform.matrix());
    style.uniform("style", shader);
    outline.uniform("outline", shader);
    map_->getTexture().use(0);
    mesh.drawArrayInstanced(sRender::TRIANGLES_STRIP, charBuffer.size());
}

glm::vec4 Text::getGlyphRect(const Glyph& glyph)const {
    FloatRect rect = glyph.rect;
    rect.w = rect.x + rect.w;
    //flip vertically
    rect.y = 1.f - rect.y;
    rect.h = rect.y - rect.h;
    return rect.vec4();
}

void Text::solve() {
    modified = 0;
    charBuffer.resize(getSize(strUnicode));
 
    glm::vec2 pos(0.f);
    size_t index = 0;
    Glyphs& glyphs = map_->metrics;
    size_t originLineY = map_.originLine();

    for (size_t i = 0; i < strUnicode.size(); i++) {

        if (strUnicode[i] == '\n') {
            //next line
            pos.y += (float)map_.sizeFont();
            pos.x = 0.f;
            continue;
        }

        Glyph& glyph = glyphs[strUnicode[i]];

        charBuffer[index].size = glyph.size;
        charBuffer[index].rect = getGlyphRect(glyph);
        charBuffer[index].model = glm::translate(glm::mat4(1.f), glm::vec3(pos + getLocalPos(glyph, (float)originLineY), 0.f));
        //to right
        pos.x += (float)glyph.advantage;
        //step charData_
        index++;
    }

    upBufferMetrica();
}
