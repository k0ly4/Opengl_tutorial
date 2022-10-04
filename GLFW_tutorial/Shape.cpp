#include "Shape.h"

///// <summary>
///// CircleShape
///// </summary>
///// <param name="size_vertex"></param>
//void CircleShape::initRenderData(size_t size_vertex)
//{
//    vertices.clear();
//    vertices.reserve((size_vertex + 2) * 2);
//    //   vertices.push_back(0.0f);
//    //   vertices.push_back(0.0f);
//    float step = 2 * PI / size_vertex;
//    for (size_t i = 0; i < size_vertex; i++) {
//        vertices.push_back(cos(step * i));
//        vertices.push_back(sin(step * i));
//    }
//    // vertices.push_back(1.0f);
//     //vertices.push_back(0.0f);
//
//    VAO.begin();
//    VBO.begin();
//
//    VBO.data(sizeof(float) * vertices.size(), vertices.data());
//    VAO.attrib(0, 2, 2 * sizeof(float), 0);
//
//    VBO.end();
//    VAO.end();
//}
//
//void CircleShape::transform() {
//    glm::vec2 size = scale * radius;
//    model = glm::mat4(1.0f);
//
//    model = glm::translate(model, glm::vec3(origin + position, 0.0f));
//    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
//
//    model = glm::translate(model, glm::vec3(-scale * origin, 0.0f));
//
//    model = glm::scale(model, glm::vec3(size, 1.0f)); // последним идет масштабирование
//}
//
//void CircleShape::draw(View* view, Shader& shader) {
//        if (need_update_model) {
//            transform();
//            need_update_model = 0;
//        }
//        shader.use();
//        view->use(shader);
//        shader.uniform("color", color);
//        shader.uniform("model", model);
//        VAO.begin();
//
//        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
//
//        if (color_outline.a > 0.f) {
//            shader.uniform("color", color_outline);
//
//            glLineWidth(outlineThickness * (scale.x + scale.y) / 2.f);
//            // glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
//            glDrawArrays(GL_LINE_LOOP, 0, vertices.size() / 2);
//            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        }
//        VAO.end();
//    }
//
///// <summary>
///// RectangleShape
///// </summary>
//void RectangleShape::transform() {
//    if (global_size != scale * local_size) {
//        global_size = scale * local_size;
//        need_update_outline = 1;
//    }
//    model_non_scale = glm::mat4(1.0f);
//
//    model_non_scale = glm::translate(model_non_scale, glm::vec3(origin + position, 0.0f));
//    model_non_scale = glm::rotate(model_non_scale, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
//
//    model_non_scale = glm::translate(model_non_scale, glm::vec3(-scale * origin, 0.0f));
//
//    model = glm::scale(model_non_scale, glm::vec3(global_size, 1.0f)); // последним идет масштабирование
//}
//void RectangleShape::initOutlineVertex() {
//        glm::vec2 vertices[5] = {
//        glm::vec2(0.f),
//        glm::vec2(global_size.x,0.f),
//           global_size,
//        glm::vec2(0.f, global_size.y),
//        glm::vec2(0.f)
//        };
//
//        glm::vec2 mat_outline_thickness[] = {
//            glm::vec2(-outlineThickness),
//            glm::vec2(outlineThickness,-outlineThickness),
//            glm::vec2(outlineThickness),
//            glm::vec2(-outlineThickness,outlineThickness)
//        };
//
//        glm::vec2 center(global_size / 2.f);
//
//        glm::vec2 outline_vertex[5];
//        for (size_t i = 0; i < 4; i++)
//            outline_vertex[i] = (vertices[i] - center) + mat_outline_thickness[i] + center;
//
//        outline_vertex[4] = outline_vertex[0];
//        glm::vec2 outlineVertices[24];
//        for (size_t i = 0, j = 0; i < 4; i++) {
//            outlineVertices[j++] = vertices[i];
//            outlineVertices[j++] = outline_vertex[i];
//            outlineVertices[j++] = outline_vertex[i + 1];
//            outlineVertices[j++] = vertices[i];
//            outlineVertices[j++] = outline_vertex[i + 1];
//            outlineVertices[j++] = vertices[i + 1];
//        }
//
//        out_VAO.begin();
//        out_VBO.begin();
//
//        out_VBO.data(sizeof(outlineVertices), outlineVertices);
//        out_VAO.attrib(0, 2, 2 * sizeof(float), 0);
//
//        out_VBO.end();
//        out_VAO.end();
//    }
//void RectangleShape::initFillVertex()
//{
//    const glm::vec2 vertices[4] = {
//     glm::vec2(0.0f, 0.0f),
//     glm::vec2(1.0f, 0.0f),
//     glm::vec2(0.0f, 1.0f),
//     glm::vec2(1.0f, 1.0f)
//    };
//    VAO.begin();
//    VBO.begin();
//
//    VBO.data(sizeof(vertices), vertices);
//    VAO.attrib(0, 2, 2 * sizeof(float), 0);
//
//    VBO.end();
//    VAO.end();
//}
//
//void RectangleShape::draw(View* view, Shader& shader) {
//        if (need_update_model) {
//            transform();
//            need_update_model = 0;
//        }
//        shader.use();
//        view->use(shader);
//        shader.uniform("color", color);
//
//
//        shader.uniform("model", model);
//        VAO.begin();
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//        VAO.end();
//        if (color_outline.a > 0.f) {
//            if (need_update_outline)
//            {
//                initOutlineVertex();
//                need_update_outline = 0;
//            }
//            out_VAO.begin();
//            shader.uniform("model", model_non_scale);
//            shader.uniform("color", color_outline);
//            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//            glDrawArrays(GL_TRIANGLES, 0, 24);
//            // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//            out_VAO.end();
//        }
//
//    }
