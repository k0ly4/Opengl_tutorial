#ifndef BILLBOARD_H
#define BILLBOARD_H
#include "PrimitiveEntity.h"

///Billboard------------------------------------------------
/// <summary>
/// 
/// </summary>
class Billboard :public Drawable
{
    
public:

    Billboard() :size_(1.f) {
        id_obj = glShader::billboard;
        VAO_.data_draw = DataDraw(DataDraw::DrawArrays, GL_POINTS, 1);
        VBO_.setMode(GL_DYNAMIC_DRAW);

        VAO_.begin();
        VBO_.begin();
        VAO_.attrib(0, 3, sizeof(glm::vec3), 0);
    }

    void setPosition(const glm::vec3& new_pos, size_t index = 0) {
        if (position.size() <= index) {
            position.resize(index + 1);
            VAO_.setCountVertexDraw(position.size());
        }
        else if (position[index] == new_pos)
            return;

        position[index] = new_pos;
        VBO_.begin();
        VBO_.data(position);
    }

    void setSize(const glm::vec2& size) {

        size_ = size;

    }

    void setEye(const View3D* eye_) {
        eye = eye_;
    }

    void setTexture(Texture2D& texture) { 
        texture_ = &texture;
    }

    void draw(const View* view, const Shader& shader) {
        if (eye) {

            shader.use();
            shader.uniform("VP", view->getVP());
            shader.uniform("right", eye->getBasis().right);
            shader.uniform("up", eye->getBasis().up);
            shader.uniform("size", size_);
            texture_->use(0);
            VAO_.draw();

        }
        else {
            LOG("(!)Billbord::eye don't set\n");
        }
    }

private:

    DrawBuffer VAO_;
    VertexBufferObject VBO_;

    const Texture2D* texture_;
    const View3D* eye = 0;
    std::vector< glm::vec3>position;
    glm::vec2 size_;

};
#endif
