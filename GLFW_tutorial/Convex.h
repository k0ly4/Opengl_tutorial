#ifndef CONVEX_H
#define CONVEX_H
#include "PrimitiveEntity.h"

class Convex :public Drawable {
    DrawBuffer VAO;
    VertexBufferObject VBO;
public:
    struct Vertex {
        glm::vec3 pos = glm::vec3(0.f);
        glm::vec3 color = glm::vec3(1.f);

    };
private:
    std::vector<Vertex> vertex;
    struct PrimitiveSetup
    {
        bool smooth = 0;
        float size = 5.f;
    }setup;

    bool need_end_primitive = 0;
    inline void EndPrimitive() {
        VAO.data_draw.data.count_vertex = vertex.size();
        VBO.begin();
        VBO.data(vertex);
        need_end_primitive = 0;
    }
public:

    Convex() {

        id_obj = glShader::m_layout_color;
        VAO.data_draw = DataDraw(DataDraw::DrawArrays, GlRender::LINE_STRIP, 0);
        VBO.setMode(GBO::DYNAMIC);
        VAO.begin();
        VBO.begin();
        VAO.attrib(0, 3, sizeof(Vertex), 0);
        VAO.attrib(1, 3, sizeof(Vertex), sizeof(glm::vec3));
    }
    void setPrimitive(GLenum mode, bool smooth, float size) {
        VAO.data_draw.data.mode_primitive = mode;
        setup = { smooth,size };
    }

    void resize(size_t new_size) {
        need_end_primitive = 1;
        vertex.resize(new_size);
    }
    void push_back(const glm::vec3& pos, const glm::vec3& color = glm::vec3(1.f)) {
        need_end_primitive = 1;
        vertex.push_back({ pos, color });
    }
    void push_back(const Vertex& _vertex) {
        need_end_primitive = 1;
        vertex.push_back(_vertex);
    }
    void pop_back() {
        need_end_primitive = 1;
        vertex.pop_back();
    }
    Vertex& operator [](size_t index) {
        need_end_primitive = 1;
        return vertex[index];
    }
    size_t size() {
        return vertex.size();
    }
    void clear() {
        need_end_primitive = 1;
        vertex.clear();
    }
    void draw(View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        if (VAO.data_draw.data.mode_primitive == GlRender::POINTS) {
            GlRender::Point::Size(setup.size);
            GlRender::Point::Smooth(1);
        }
        else if (VAO.data_draw.data.mode_primitive < GlRender::TRIANGLES) {
            GlRender::Line::Width(setup.size);
            GlRender::Line::Smooth(1);
        }

        shader.uniform("model", glm::mat4(1.f));
        if (need_end_primitive)EndPrimitive();
        VAO.draw();
    }
};

#endif