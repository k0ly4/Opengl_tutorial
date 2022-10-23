#ifndef MESH_H
#define MESH_H

#include "Resource/Assimp.h"
#include "Graphic/Render.h"
#include "Graphic/Texture.h"
#include <string>
#include <vector>

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

    // ������ ����
    void addAssets(aiMaterial* mat, const std::string& directory, aiTextureType type, Asset::TYPE typeName, bool gammaCorrection);
    void addMaterial(aiMaterial* material, const std::string& directory, bool gammaCorrection);
    void bindAssets(const Shader& shader); 
};

/// <summary>
/// VertexMesh
/// </summary>
class VertexMesh:public MaterialMesh,public Drawable {

protected:
    // ������ ��� ���������� 
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    DrawBuffer VAO;
    VertexMesh() {}
    VertexMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gammaCorreciton);
    void setupVO();

private:

    VertexBufferObject VBO;
    ElementBufferObject EBO;
    // ���� �� ���� �������� ����
    void setVertex(aiMesh* mesh);
    // ���������� �� ������ ����� ���� (����� - ��� ����������� ����) � ��������� ��������������� ������� ������
    void setIndices(aiMesh* mesh);
};

/// <summary>
/// Mesh
/// </summary>
class Mesh :public VertexMesh,public Transformable3D {
   
public:

    Mesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gammaCorreciton);
 // ��������� ����
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
        // ������������ ���
        VAO.draw();
    }

    void draw(const View* view, const Shader& shader);

private:

    std::string name;

    void init_data_draw() {
        shaderHint = glShader::any;
        VAO.data_draw = DataDraw(DataDraw::DrawElements, GL_TRIANGLES, indices.size());
    }  
}; 

#endif