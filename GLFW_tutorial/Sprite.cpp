#include "Sprite.h"

void Sprite::transform() {
    glm::vec2 size = scale * size_;
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(origin + position, 0.0f));
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::translate(model, glm::vec3(-scale * origin, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f)); // последним идет масштабирование
}

void Sprite::setTexture(const Texture2D& texture) {
    texture_ = &texture;
    size_ = texture.getSize();
    need_update_model = 1;
}

void Sprite::setTextureRect(const FloatRect& cut_rect) {
    const glm::ivec2& texSize = texture_->getSize();

    model_uv = glm::translate(glm::mat4(1.f),
        glm::vec3(cut_rect.x / texSize.x, (texSize.y - cut_rect.h - cut_rect.y) / texSize.y, 0.f));

    model_uv = glm::scale(model_uv,
        glm::vec3(cut_rect.w / texSize.x, cut_rect.h / texSize.y, 0.f));

    size_.x = cut_rect.w;
    size_.y = cut_rect.h;
    need_update_model = 1;
}

void Sprite::draw(const View* view,const Shader& shader) {
    shader.use();
    view->use(shader);
    shader.uniform("color", color_);
    shader.uniform("model_uv", model_uv);
    if (need_update_model) {
        transform();
        need_update_model = 0;
    }
    shader.uniform("model", model);
    texture_->use(0);
    sBuffer::quad2D.getVAO().draw();
}