#ifndef BILLBOARD_H
#define BILLBOARD_H
#include "PrimitiveEntity.h"
class Billboard :public Drawable
{
    DrawBuffer VAO;
    VertexBufferObject VBO;
    Texture2D* texture;
    const View3D* eye = 0;
    std::vector< glm::vec3>position;
    glm::vec2 size;

public:
    Billboard() :size(1.f) {
        id_obj = glShader::billboard;
        VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_POINTS, 1);
        VBO.setMode(GL_DYNAMIC_DRAW);
        VAO.begin();
        VBO.begin();
        VAO.attrib(0, 3, sizeof(glm::vec3), 0);
    }
    void setPosition(const glm::vec3& new_pos, size_t index = 0) {
        if (position.size() <= index) {
            position.resize(index + 1);
            VAO.setCountVertexDraw(position.size());
        }
        else if (position[index] == new_pos)return;
        position[index] = new_pos;
        VAO.begin();
        VBO.begin();
        VBO.data(position);
    }
    void setSize(const glm::vec2& size) {
        this->size = size;
    }
    void setEye(const View3D* eye_) {
        eye = eye_;
    }
    void setTexture(Texture2D& texture) { this->texture = &texture; }
    void draw(View* view, const Shader& shader) {
        if (eye) {
            shader.use();
            shader.uniform("VP", view->getVP());
            shader.uniform("right", eye->getBasis().right);
            shader.uniform("up", eye->getBasis().up);
            shader.uniform("size", size);
            texture->use(0);
            VAO.draw();
        }
        else {
            printf("(!)Billbord::eye don't set\n");
        }
    }
};
#endif
