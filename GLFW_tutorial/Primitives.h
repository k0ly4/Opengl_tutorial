#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include"Framebuffer.h"

class Primitive {
    static std::vector<VertexBufferObject> vbo;
    Primitive() {}
    ~Primitive() {}
    static void cube_data(VertexBufferObject& vbo) {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        vbo.data(sizeof(vertices), vertices);
    }
    static void quad_data(VertexBufferObject& vbo) {
        float vertices[] = {
            // координаты      // текстурные координаты
           -1.0f,  1.0f, 0.0f,      0.0f, 1.0f,
           -1.0f, -1.0f, 0.0f,      0.0f, 0.0f,
            1.0f,  1.0f, 0.0f,      1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
        };
        vbo.data(sizeof(vertices), vertices);
    }
    static void quad2d_data(VertexBufferObject& vbo) {
        float vertices[] = {
      0.0f, 0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 0.0f
        };
        vbo.data(sizeof(vertices), vertices);
    }
    static void plane_data(VertexBufferObject& vbo) {
        //float vertices[] = {
        //    // координаты        // текстурные координаты (y-координаты поменялись местами, потому что текстура перевернута вверх ногами)
        //    -0.5f,  -0.5f,0.0f,  0.0f,  0.0f,
        //    0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
        //    -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,
        //    0.5f,  0.5f,  0.0f,  1.0f,  1.0f
        //};
        float planeVertices[] = {
            // positions            // normals         // texcoords
            -0.5f,  -0.5f,0.0f, 0.f,1.f,0.f,       0.0f,  0.0f,
            0.5f, -0.5f,  0.0f, 0.f,1.f,0.f,      1.0f,  0.0f,
            -0.5f, 0.5f,  0.0f, 0.f,1.f,0.f,       0.0f,  1.0f,
            0.5f,  0.5f,  0.0f, 0.f,1.f,0.f,       1.0f,  1.0f
        };
        vbo.data(sizeof(planeVertices), planeVertices);
    }
public:
    enum VBO_OBJECT :size_t
    {
        cube, quad, quad2d, plane, vbo_size
    };
    
    class Cube :public Drawable, public Transformable3D {
        Texture2D* texture = 0;
        glm::vec3 color = glm::vec3(1.f);
        friend class Primitive;
        static void init() {
            VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLES, 36);
            vbo[cube].begin();
            VAO.begin();
            VAO.attrib(0, 3, 8 * sizeof(float), 0);
            VAO.attrib(1, 2, 8 * sizeof(float), 6 * sizeof(float));
        }
    public:
        static DrawBuffer VAO;
        Cube() {
            id_obj = glShader::gb_color_uniform;
        }
        void setColor(const glm::vec3& _color) {
            color = _color;
            texture = 0;
            id_obj = glShader::gb_color_uniform;
        }
        void setTexture(Texture2D& texture) {
            id_obj = glShader::gb_texturable;
            this->texture = &texture;
        }
        void draw(View* view, const Shader& shader) {
            shader.use();
            view->use(shader);
            if (texture)  texture->use(0);
            else shader.uniform("color", color);
            uniformTransform(shader);
            VAO.draw();
        }
    };
    class gbCube :public gbMateriable, public Transformable3D {
            friend class Primitive;
            static void init() {
                VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLES, 36);
                vbo[cube].begin();
                VAO.begin();
                VAO.attrib(0, 3, 8 * sizeof(float), 0);
                VAO.attrib(1, 3, 8 * sizeof(float), 3 * sizeof(float)); 
                VAO.attrib(2, 2, 8 * sizeof(float), 6 * sizeof(float));
            }
        public:
            static DrawBuffer VAO;
            gbCube() {
                id_obj = glShader::gb_color_uniform;
            }
            void draw(View* view, const Shader& shader) {
                shader.use();
                view->use(shader);
                uniformMaterial(shader);
                uniformTransform(shader);
                VAO.draw();
            }
    };

    class Quad2D :public Drawable {
        static DrawBuffer VAO;
        Texture2D* texture;
        friend class Primitive;
        static void init() {
            VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLE_STRIP, 4);
            VAO.begin();
            vbo[quad2d].begin();
            VAO.attrib(0, 4, 4 * sizeof(float), 0);
        }
        float exposure = 2.2f;
    public:
        Quad2D() {
            id_obj = glShader::frame_exposure;
        }
        void setTexture(Texture2D& texture) {
            this->texture = &texture;
        }
        void draw(const View* view, const Shader& shader) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            texture->use(0);
            shader.use();
            shader.uniform("exposure", exposure);

            GlRender::CullFace::Enable(false);
            VAO.draw();
        }
    };
    class Quad :public Drawable {

        Texture2D* texture;
        friend class Primitive;
        static void init() {
            VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLE_STRIP, 4);
            VAO.begin();
            vbo[quad].begin();
            VAO.attrib(0, 3, 5 * sizeof(float), 0);
            VAO.attrib(1, 2, 5 * sizeof(float), 3 * sizeof(float));
        }
        float exposure = 2.2f;
    public:
        static DrawBuffer VAO;
        Quad() {
            id_obj = glShader::frame_exposure;
        }
        void setTexture(Texture2D& texture) {
            this->texture = &texture;
        }
        void draw(const View* view, const Shader& shader) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            texture->use(0);
            shader.use();
            shader.uniform("exposure", exposure);

            GlRender::CullFace::Enable(false);
            VAO.draw();
        }
    };
    class Plane :public Drawable, public Transformable3D {
        Texture2D* texture;
        friend class Primitive;
        static void init() {
            VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLE_STRIP, 4);
            vbo[plane].begin();
            VAO.begin();
            VAO.attrib(0, 3, 8 * sizeof(float), 0);
            VAO.attrib(1, 2, 8 * sizeof(float), 6 * sizeof(float));

        }
    public:
        static DrawBuffer VAO;
        Plane() {
            id_obj = glShader::m_texture;
        }
        void setTexture(Texture2D& texture) {
            this->texture = &texture;
        }
        void draw(View* view, const Shader& shader) {
            shader.use();
            view->use(shader);
            uniformTransform(shader);
            texture->use(0);
            VAO.draw();
        }
    };
    class gbPlane :public gbMateriable, public Transformable3D {
        friend class Primitive;
        static void init() {
            VAO.data_draw = DataDraw(DataDraw::DrawArrays, GL_TRIANGLE_STRIP, 4);
            vbo[plane].begin();
            VAO.begin();
            VAO.attrib(0, 3, 8 * sizeof(float), 0);
            VAO.attrib(1, 3, 8 * sizeof(float), 3 * sizeof(float));
            VAO.attrib(2, 2, 8 * sizeof(float), 6 * sizeof(float));
        }
    public:
        static DrawBuffer VAO;
        gbPlane() {
            id_obj = glShader::gb_color_uniform;
        }
        void draw(View* view, const Shader& shader) {
            shader.use();
            view->use(shader);
            uniformMaterial(shader);
            uniformTransform(shader);
            VAO.draw();
        }
    };
    class Filter {
        float exposure = 2.2f;
        void Exposure(const Shader& shader, const Texture2D& texture) {
            shader.use();
            shader.uniform("exposure", exposure);
            texture.use(0);
            Quad::VAO.draw();
        }
        void Red(const Shader& shader, const Texture2D& texture) {
            shader.use();
            texture.use(0);
            Quad::VAO.draw();
        }
        void Red(const Shader& shader, const g_ArrayTexture2D& texture,int level) {
            shader.use();
            texture.use(0);
            shader.uniform("level", level);
            Quad::VAO.draw();
        }
    public:
        void displayRed(const Texture2D& texture) {
            Red(glShader::get(glShader::red), texture);
        }
        void displayRed(const g_ArrayTexture2D& texture,int level) {
            Red(glShader::get(glShader::red_array), texture,level);
        }
        void setExposure(float exp) {
            exposure = exp;
        }
        void drawExposure(const Texture2D& texture) {
            Exposure(glShader::get(glShader::frame_exposure), texture);
        }
    };
    class Skybox :public Drawable {
        TextureCubeMap texture;
        static DrawBuffer VAO;
        static VertexBufferObject VBO;
        friend class Primitive;
        static void init() {
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
    public:
        Skybox() {
            id_obj = glShader::skybox;
        }
        void load(const std::string& directory, bool flip_vertically = 1, bool gamma = 0) {
            texture.load(directory, flip_vertically, gamma);
        }

        void draw(View* view, const Shader& shader) {
            shader.use();
            shader.uniform("projection", view->getProjection().Matrix());
            shader.uniform("camera", glm::mat4(glm::mat3(view->getMatrixView().Matrix())));
            texture.use(0);
            glDepthFunc(GL_LEQUAL);
            VAO.draw();
            glDepthFunc(GL_LESS);
        }
    };
    class Billboard :public Drawable
    {
        DrawBuffer VAO;
        VertexBufferObject VBO;
        Texture2D* texture;
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
        void setTexture(Texture2D& texture) { this->texture = &texture; }
        void draw(View* view, const Shader& shader) {
            shader.use();
            shader.uniform("VP", view->getVP());
            shader.uniform("right", view->getMatrixView().Basis().right);
            shader.uniform("up", view->getMatrixView().Basis().up);
            shader.uniform("size", size);
            texture->use(0);
            VAO.draw();
        }
    };
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
    static void init() {
        vbo.resize(vbo_size);
        vbo[cube].begin();
        cube_data(vbo[cube]);

        vbo[quad].begin();
        quad_data(vbo[quad]);

        vbo[quad2d].begin();
        quad2d_data(vbo[quad2d]);

        vbo[plane].begin();
        plane_data(vbo[plane]);


        Plane::init();
        gbPlane::init();
        Cube::init();
        gbCube::init();
        Quad::init();
        Skybox::init();
    }

    static VertexBufferObject& get(size_t index) {
        return vbo[index];
    }
};
std::vector<VertexBufferObject> Primitive::vbo;
DrawBuffer Primitive::Cube::VAO;

DrawBuffer Primitive::gbCube::VAO;
DrawBuffer Primitive::Quad::VAO;
DrawBuffer Primitive::Plane::VAO;
DrawBuffer Primitive::gbPlane::VAO;
DrawBuffer Primitive::Skybox::VAO;
VertexBufferObject Primitive::Skybox::VBO;
#endif