#ifndef CONVEX_H
#define CONVEX_H
#include "Resource/ShaderArguments.h"
#include "Graphic/Geometry.h"
#include "Resource/Shader.h"
template<typename T>
class MeshShell {

public:

    MeshShell() { mesh.VBO.setMode(GBO::DYNAMIC); }

    inline void resize(size_t new_size) {
        modified = 1;
        mesh.vertices.resize(new_size);
    }

    inline void pop_back() {
        modified = 1;
        mesh.vertices.pop_back();
    }

    inline T& operator [](size_t index) {
        modified = 1;
        return mesh.vertices[index];
    }

    inline size_t size()const { return mesh.vertices.size(); }
    inline void push_back(const T& vertex) {
        modified = 1;
        mesh.push_back(vertex);
    }
    inline void clear() {
        modified = 1;
        mesh.clear();
    }
    inline void draw(const RenderTarget& target,size_t primitive = Render::LINES, glShader::Object shaderHint = glShader::color_layout) {
        const Shader& shader = glShader::get(shaderHint);
        shader.use();
        target.getView()->use(shader);
        shader.uniform("model", model);
        if (modified)mesh.saveInBuffer();
        mesh.drawArrays(primitive);
    }  
    glm::mat4 model = glm::mat4(1.f);

private:

    bool modified = 0;
    qGeometry<T> mesh;
};
/// <summary>
/// Convex
/// </summary>
class Convex :public Drawable {

   
public:

    struct Setup
    {
        bool smooth = 0;
        float size = 5.f;
    };

    Convex();

    void setPrimitive(GLenum mode, bool smooth, float size) {
        VAO.data_draw.data.mode_primitive = mode;
        setup = { smooth,size };
    }

    void resize(size_t new_size) {
        need_end_primitive = 1;
        vertices.resize(new_size);
    }

    void push_back(const glm::vec3& pos, const glm::vec4& color = glm::vec4(1.f)) {
        need_end_primitive = 1;
        vertices.push_back({ pos, color });
    }

    void push_back(const ConvexVertex& _vertex) {
        need_end_primitive = 1;
        vertices.push_back(_vertex);
    }

    void pop_back() {
        need_end_primitive = 1;
        vertices.pop_back();
    }
    ConvexVertex& operator [](size_t index) {
        need_end_primitive = 1;
        return vertices[index];
    }
    size_t size()const {
        return vertices.size();
    }
    void clear() {
        need_end_primitive = 1;
        vertices.clear();
    }
    void draw(const View* view, const Shader& shader);

private:

    Setup setup;
    std::vector<ConvexVertex> vertices;
    DrawBuffer VAO;
    VertexBufferObject VBO;
    bool need_end_primitive = 0;

    inline void EndPrimitive() {
        VAO.data_draw.data.count_vertex = vertices.size();
        VBO.begin();
        VBO.data(vertices);
        need_end_primitive = 0;
    }
};

/// <summary>
/// Convex
/// </summary>
template<typename T>
class TypeConvex {

public:

    TypeConvex()
    {
        vertices_.reserve(10);
        VAO_.data_draw = DataDraw(DataDraw::DrawArrays, Render::TRIANGLES, 0);
        VBO_.setMode(GBO::DYNAMIC);
        VBO_.begin();
        VAO_.begin();
        T::attrib(VAO_);
    }

    TypeConvex(size_t amount_vertex) {
        vertices_.resize(amount_vertex);
        VAO_.data_draw = DataDraw(DataDraw::DrawArrays, Render::TRIANGLES, amount_vertex);
        VBO_.setMode(GBO::DYNAMIC);
        VBO_.begin();
        VAO_.begin();
        T::attrib(VAO_);
    }

    void setDataDraw(const DataDraw& data) {
        VAO_.data_draw = data;
    }

    void resize(size_t new_size) {
        modified = 1;
        vertices_.resize(new_size);
    }

    void push_back(const T& vertex) {
        vertices_.push_back(vertex);
        modified = 1;
    }

    void pop_back() {
        vertices_.pop_back();
        modified = 1;
    }

    T& operator [](size_t index) {
        modified = 1;
        return vertices_[index];
    }

    inline size_t size()const {
        return vertices_.size();
    }

    void clear() {
        vertices_.clear();
        modified = 1;
    }

    void draw() {
        if (modified) endPrimitive();
        VAO_.draw();
    }

    DrawBuffer& getVAO()const {
        if (modified) endPrimitive();
        return VAO_;
    }

private:

    std::vector<T> vertices_;
    mutable DrawBuffer VAO_;
    mutable VertexBufferObject VBO_;
    mutable bool modified = 1;

    inline void endPrimitive()const {
        VAO_.data_draw.data.count_vertex = size();
        VAO_.begin();
        VBO_.begin();
        VBO_.data(vertices_);
        modified = 0;
    }

};
/// <summary>
/// Convex
/// </summary>
class ConvexUV {


public:

    ConvexUV()
 
    {
        vertices_.reserve(10);
        VAO_.data_draw = DataDraw(DataDraw::DrawArrays, Render::TRIANGLES_STRIP, 0);
        VBO_.setMode(GBO::DYNAMIC);
        VBO_.begin();
        VAO_.begin();
        VAO_.attrib(0, 3, sizeof(UvVertex), 0);
        VAO_.attrib(1, 2, sizeof(UvVertex), sizeof(glm::vec3));
    }

    ConvexUV(size_t amount_vertex) {
        vertices_.resize(amount_vertex);
        VAO_.data_draw = DataDraw(DataDraw::DrawArrays, Render::TRIANGLES_STRIP, amount_vertex);
        VBO_.setMode(GBO::DYNAMIC);
        VBO_.begin();
        VAO_.begin();
        VAO_.attrib(0, 3, sizeof(UvVertex), 0);
        VAO_.attrib(1, 2, sizeof(UvVertex), sizeof(glm::vec3));
    }

    void setDataDraw(const DataDraw& data) {
        VAO_.data_draw = data;
    }

    void resize(size_t new_size) {
        need_end_primitive = 1;
        vertices_.resize(new_size);
    }

    void push_back(const glm::vec3& pos, const glm::vec2& uv) {
        vertices_.push_back({ pos, uv });
        need_end_primitive = 1;
    }

    void push_back(const UvVertex& _vertex) {
        vertices_.push_back(_vertex);
        need_end_primitive = 1;
    }

    void pop_back() {
        vertices_.pop_back();
        need_end_primitive = 1;
    }

    UvVertex& operator [](size_t index) {
        need_end_primitive = 1;
        return vertices_[index];
    }

    size_t size()const {
        return vertices_.size();
    }

    void clear() {
        vertices_.clear();
        need_end_primitive = 1;
    }

    void draw() {
        if (need_end_primitive) endPrimitive();
        VAO_.draw();
    }

    DrawBuffer& getVAO()const {
        if (need_end_primitive) endPrimitive();
        return VAO_;
    }

private:

    std::vector<UvVertex> vertices_;
    mutable DrawBuffer VAO_;
    mutable VertexBufferObject VBO_;
    mutable bool need_end_primitive = 1;

    inline void endPrimitive()const {
        VAO_.data_draw.data.count_vertex = vertices_.size();
        VAO_.begin();
        VBO_.begin();
        VBO_.data(vertices_);
        need_end_primitive = 0;
    }

};
#endif