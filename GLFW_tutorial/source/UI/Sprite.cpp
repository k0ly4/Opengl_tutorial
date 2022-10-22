#include "Sprite.h"

void Sprite::setTexture(const Texture2D& texture) {
    if (material_.getTexture() == &texture) return;
    material_.setTexture(texture);
    setTextureRect(FloatRect(glm::vec2(0.f), texture.getSize()));
}

void Sprite::setTextureRect(const FloatRect& rect) {
    if (textureRect_ == rect) return;
    textureRect_ = rect;
    needUpConvex_ = 1;
}

void Sprite::draw(const View* view, const Shader& shader) {
    if (needUpConvex_)
        upConvex();
    shader.use();
    view->use(shader);
    uniformTransform(shader);
    uniformMaterial(shader);
    convex_.draw();
}

void Sprite::upConvex() {
    FloatRect normalize = textureRect_ / material_.getTexture()->getSize();

    normalize.w = normalize.x + normalize.w;
    //flip vertically
    normalize.y = 1.f - normalize.y;
    normalize.h = normalize.y - normalize.h;

    convex_[0] = UvVertex(glm::vec3(0.f, textureRect_.h, 0.0f),             glm::vec2(normalize.x, normalize.h));      //glm::vec2(0.f,0.f)
    convex_[1] = UvVertex(glm::vec3(0.f, 0.f, 0.0f),                        glm::vec2(normalize.x, normalize.y));      //glm::vec2(0.f,1.f)
    convex_[2] = UvVertex(glm::vec3(textureRect_.w, textureRect_.h, 0.0f),  glm::vec2(normalize.w, normalize.h));      //glm::vec2(1.f,0.f)
    convex_[3] = UvVertex(glm::vec3(textureRect_.w, 0.f, 0.0f),             glm::vec2(normalize.w, normalize.y));      //glm::vec2(1.f,1.f)

    needUpConvex_ = 0;
}