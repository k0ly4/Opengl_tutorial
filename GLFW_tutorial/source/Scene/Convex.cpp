#include "Convex.h"

/// <summary>
/// Convex
/// </summary>
/// 
Convex::Convex() {

    shaderHint = glShader::color_layout;
    VAO.data_draw = DataDraw(DataDraw::DrawArrays, GlRender::LINE_STRIP, 0);
    VBO.setMode(GBO::DYNAMIC);
    VAO.begin();
    VBO.begin();
    VAO.attrib(0, 3, sizeof(ConvexVertex), 0);
    VAO.attrib(1, 3, sizeof(ConvexVertex), sizeof(glm::vec3));

}

void Convex::draw(const View* view, const Shader& shader) {
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