#include "Sprite.h"

void Sprite::setTexture(const Texture2D& texture) {
    if (texture_ == &texture) return;
    texture_ = &texture;
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
    texture_->use(0);
    shader.uniform("color", color_);
    convex_.draw();
}