#include "Shape.h"

 //<summary>
 //CircleShape
 //</summary>
 //<param name="size_vertex"></param>
void CircleShape::initRenderData(size_t size_vertex)
{
    mesh.vertices.clear();
    mesh.vertices.reserve((size_vertex + 2) * 2);
    mesh.push_back(glm::vec2(0.f));
    float step = 2 * PI / size_vertex;
    for (size_t i = 0; i < size_vertex; i++) mesh.push_back(glm::vec2(cos(step*i), sin(step * i)));
    
    mesh.push_back(glm::vec2(1.0f,0.f));
    mesh.saveInBuffer();
}
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

void CircleShape::draw(const View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        shader.uniform("color", color);
        shader.uniform("model", transform.matrix()); 
        mesh.drawArrays(GlRender::TRIANGLES_FAN);
        /*if (color_outline.a > 0.f) {
            shader.uniform("color", color_outline);

            glLineWidth(outlineThickness * (scale.x + scale.y) / 2.f);
             glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDrawArrays(GL_LINE_LOOP, 0, vertices.size() / 2);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        VAO.end();*/
    }

 /*<summary>
 RectangleShape
 </summary>*/
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
void RectangleShape::solve()
{
    modified = 0;
    mesh.vertices.resize(20);
    mesh.indices.clear();
    //rectangle
    mesh.pushSquare(0);
    mesh.vertices[0] = {glm::vec2(0.0f, 0.0f),                                      color };
    mesh.vertices[1] ={glm::vec2(size_.x, 0.0f  ),                                  color };
    mesh.vertices[2] ={glm::vec2(size_.x,size_.y),                                  color };
    mesh.vertices[3] ={glm::vec2(0.0f,    size_.y),                                 color };
    //top line
    mesh.pushSquare(4);
    mesh.vertices[4] ={ glm::vec2(-size_out,    -size_out),                         color_out };
    mesh.vertices[5] ={ glm::vec2(size_.x+ size_out,      -size_out),               color_out };
    mesh.vertices[6] ={ glm::vec2(size_.x,            0.f),                         color_out };
    mesh.vertices[7] ={ glm::vec2(0.f,                0.f),                         color_out };
    //right line
    mesh.pushSquare(8);
    mesh.vertices[8] ={ glm::vec2(size_.x,    0.f),                                 color_out };
    mesh.vertices[9] ={ glm::vec2(size_.x + size_out,      -size_out),              color_out };
    mesh.vertices[10] ={ glm::vec2(size_.x + size_out,size_.y + size_out),          color_out };
    mesh.vertices[11] ={ glm::vec2(size_.x,size_.y),                                color_out };
    //down line
    mesh.pushSquare(12);
    mesh.vertices[12] ={ glm::vec2(0.f,size_.y),                                    color_out };
    mesh.vertices[13] ={ glm::vec2(size_.x,size_.y),                                color_out };
    mesh.vertices[14] ={ glm::vec2(size_.x + size_out,size_.y + size_out),          color_out };
    mesh.vertices[15] ={ glm::vec2(-size_out,size_.y + size_out),                   color_out };
    //left line
    mesh.pushSquare(16);
    mesh.vertices[16] ={ glm::vec2(-size_out,    -size_out),                        color_out };
    mesh.vertices[17] ={ glm::vec2(0.f,0.f),                                        color_out };
    mesh.vertices[18] ={ glm::vec2(0.f,size_.y),                                    color_out };
    mesh.vertices[19] ={ glm::vec2(-size_out,size_.y + size_out),                   color_out };

    mesh.saveInBuffer();
}

void RectangleShape::draw(const View* view, const Shader& shader) {
        if (modified)solve();
        shader.use();
        view->use(shader);
        shader.uniform("model", transform.matrix());
        mesh.draw();
    }
