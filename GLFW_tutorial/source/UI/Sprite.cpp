#include "Sprite.h"

void Sprite::setTexture(const Texture2D& texture_) {
    if (texture == &texture_) return;
    texture = &texture_;
    setTextureRect(FloatRect(glm::vec2(0.f), texture_.getSize()));
}

void Sprite::setTextureRect(const FloatRect& rect) {
    if (textureRect_ == rect) return;
    textureRect_ = rect;
    modified = 1;
}

void Sprite::draw(const View* view, const Shader& shader) {
   if (modified)solve();
   shader.use();
   view->use(shader);
   texture->use(0);
   shader.uniform("model", transform.matrix());
   shader.uniform("color", color_);

   mesh.drawArrays(Render::TRIANGLES_STRIP);
}

void Sprite::solve() {
    modified = 0;
    //Normalize
    FloatRect normalize = textureRect_ / texture->getSize();
    normalize.y = 1.f - normalize.y;
    //Mesh
    normalize.w = normalize.x + normalize.w;
    normalize.h = normalize.y - normalize.h;
    mesh.vertices[0] = glm::vec4(0.f, textureRect_.h,             normalize.x, normalize.h);      //glm::vec2(0.f,0.f)
    mesh.vertices[1] = glm::vec4(0.f, 0.f,                        normalize.x, normalize.y);      //glm::vec2(0.f,1.f)
    mesh.vertices[2] = glm::vec4(textureRect_.w, textureRect_.h,  normalize.w, normalize.h);      //glm::vec2(1.f,0.f)
    mesh.vertices[3] = glm::vec4(textureRect_.w, 0.f,             normalize.w, normalize.y);      //glm::vec2(1.f,1.f)

    mesh.saveInBuffer();
}