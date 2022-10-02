#ifndef MESH_H
#define MESH_H

#include "Assimp.h"
#include "Render.h"
#include "Texture.h"
#include <string>
#include <vector>

struct Vertex {

    // Позиция
    glm::vec3 position;

    // Нормаль
    glm::vec3 normal;

    // Текстурные координаты
    glm::vec2 texCoord;
};
#define SIZE_VERTEX sizeof(Vertex)

/// <summary>
/// Asset
/// </summary>
struct Asset {
    Texture2D texture;
    enum TYPE :size_t {
        diffuse, specular, normal, height, size
    }type;
    Asset() {}
    Asset(const std::string& path, TYPE type, bool gamma = false) {
        load(path, gamma);
        this->type = type;
    }
    bool load(const std::string& path, bool gamma = false);
};

/// <summary>
/// MaterialMesh
/// </summary>
class MaterialMesh{

public:

    inline bool haveTexture() {
        return texturable;
    }

protected:

    bool texturable;
    void uniformMaterial(const Shader& shader);
    void setMaterials(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gamma);

private:

    const std::string name_shader_uniform[Asset::TYPE::size] = {
               "diffuse","specular","normal","height"
    }; 
    std::vector<Asset> texture;
    float specularFactor;
    glm::vec3 color;

    // Данные меша
    void addAssets(aiMaterial* mat, const std::string& directory, aiTextureType type, Asset::TYPE typeName, bool gammaCorrection);
    void addMaterial(aiMaterial* material, const std::string& directory, bool gammaCorrection);
    void bindAssets(const Shader& shader); 
};

/// <summary>
/// VertexMesh
/// </summary>
class VertexMesh:public MaterialMesh {

protected:
    // Данные для рендеринга 
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    DrawBuffer VAO;
    VertexMesh() {}
    VertexMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gammaCorreciton);
    void setupVO();

private:

    VertexBufferObject VBO;
    ElementBufferObject EBO;
    // Цикл по всем вершинам меша
    void setVertex(aiMesh* mesh);
    // Проходимся по каждой грани меша (грань - это треугольник меша) и извлекаем соответствующие индексы вершин
    void setIndices(aiMesh* mesh);
};

/// <summary>
/// Mesh
/// </summary>
class Mesh :public VertexMesh, public Instancable {
   
public:

    Mesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gammaCorreciton);
 // Рендеринг меша
    inline const std::string& getName() {
        return name;
    }

    const DrawBuffer& getDrawBuffer() {
        return VAO;
    }

    inline void uniform(const Shader& shader) {
        uniformMaterial(shader);
    }

    void draw(const Shader& shader)
    {
        uniform(shader);
        // Отрисовываем меш
        VAO.draw();
    }

    void draw(const View* view, const Shader& shader);

private:

    std::string name;

    void init_data_draw() {
        id_obj = glShader::m_texture_normal;
        VAO.data_draw = DataDraw(DataDraw::DrawElements, GL_TRIANGLES, indices.size());
    }  
}; 

#endif