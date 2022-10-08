#ifndef SKYBOX_H
#define SKYBOX_H
#include "PrimitiveEntity.h"
class Skybox :public Drawable {
   
public:

    Skybox() {
        shaderHint = glShader::skybox;
    }

    void load(const std::string& directory, bool flip_vertically = 1, bool gamma = 0) {
        texture.loadFromDirectory(directory, flip_vertically, gamma);
    }

    void draw(View* view, const Shader& shader) {
        shader.use();
        shader.uniform("projection", view->getProjection().get());
        shader.uniform("camera", glm::mat4(glm::mat3(view->getView().get())));
        texture.use(0);

        Depth::Func(Depth::Lequal);
        VAO.draw();
        Depth::Func(Depth::Less);
    }

private:

    TextureCubeMap texture;

    DrawBuffer VAO;
    VertexBufferObject VBO;

    void init() {
        float vertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLES, 36);
        VAO.begin();
        VBO.begin();
        VBO.data(sizeof(vertices), vertices);
        VAO.attrib(0, 3, 3 * sizeof(float), 0);
        VBO.end();
        VAO.end();
    }
};
#endif
