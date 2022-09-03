#ifndef MESH_H
#define MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Render.h"
#include "Texture.h"
#include <string>
#include <vector>
template<typename mat2, typename mat1>
void convertMatrix(size_t size,const mat1& m1,mat2& m2) {
    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++)
            m2[i][j] = m1[j][i];
}

void aiMatrix4x4_to_mat4(const aiMatrix4x4& mat1, glm::mat4& mat2) {
    convertMatrix(4, mat1, mat2);
}
void aiMatrix3x3_to_mat3(const aiMatrix3x3& mat1, glm::mat3& mat2) {
    convertMatrix(3, mat1, mat2);
}
glm::vec3 to_vec3(const aiColor4D& vec) {
    return glm::vec3(vec.r, vec.g, vec.b);
}
glm::vec3 to_vec3(const aiVector3D& vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}
glm::vec2 to_vec2(const aiVector3D& vec) {
    return glm::vec2(vec.x, vec.y);
}
glm::mat4 to_mat4(const aiMatrix4x4& mat) {
    glm::mat4 result;
    aiMatrix4x4_to_mat4(mat, result);
    return result;
}
glm::mat3 to_mat3(const aiMatrix3x3& mat) {
    glm::mat3 result;
    aiMatrix3x3_to_mat3(mat, result);
    return result;
}

void print(const aiMatrix4x4& mat) {
    printf("    %f %f %f %f\n", mat.a1, mat.a2, mat.a3, mat.a4);
    printf("    %f %f %f %f\n", mat.b1, mat.b2, mat.b3, mat.b4);
    printf("    %f %f %f %f\n", mat.c1, mat.c2, mat.c3, mat.c4);
    printf("    %f %f %f %f\n", mat.d1, mat.d2, mat.d3, mat.d4);

}

struct Vertex {

    // �������
    glm::vec3 position;

    // �������
    glm::vec3 normal;

    // ���������� ����������
    glm::vec2 texCoord;
};
#define SIZE_VERTEX sizeof(Vertex)

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
    bool load(const std::string& path, bool gamma = false) {
        GeneralTexture* text = FileManager::getLoadedTexture(path);
        if (text) {
            std::cout << "Texture already had created " << path << "\n";
            texture = Texture2D(*text, glm::uvec2(0));
            return 1;
        }
        return texture.loadFromFile(path, 1, gamma);
    }
};
class MaterialMesh{
    // ������ ����
    void addAssets(aiMaterial* mat, const std::string& directory, aiTextureType type, Asset::TYPE typeName, bool gammaCorrection)
    {
        aiString str;

        for (size_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            mat->GetTexture(type, i, &str);
            std::cout << str.C_Str() << std::endl;
            texture.push_back(Asset(directory + '/' + std::string(str.C_Str()), typeName, gammaCorrection));
        }
    }
    void addMaterial(aiMaterial* material, const std::string& directory, bool gammaCorrection) {
        // �� ������ ���������� �� ������ ��������� � ��������. ������ ��������� �������� ����� ���������� 'texture_diffuseN',
        // ��� N - ��� ���������� ����� �� 1 �� MAX_SAMPLER_NUMBER. 
        // �� �� ����� ��������� � � ������ ���������:
        // ���������: texture_diffuseN
        // ���������: texture_specularN
        // �������: texture_normalN
        // 1. ��������� �����
        addAssets(material, directory, aiTextureType_DIFFUSE, Asset::diffuse, gammaCorrection);
        // 2. ����� ���������
        addAssets(material, directory, aiTextureType_SPECULAR, Asset::specular, 0);
        // 3. ����� ��������
        addAssets(material, directory, aiTextureType_HEIGHT, Asset::normal, 0);
        // 4. ����� �����
        addAssets(material, directory, aiTextureType_AMBIENT, Asset::height, 0);
    }
    const std::string name_shader_uniform[Asset::TYPE::size] = {
                "diffuse","specular","normal","height"
    };
    void bindAssets(const Shader& shader) {
        unsigned int num_text[Asset::TYPE::size] = { 1,1,1,1 };
        for (size_t i = 0; i < texture.size(); i++)
        {
            // �������� texture �������� (����� N � diffuse_textureN)
            num_text[texture[i].type]++;
            // ������ ������������� ������� �� ������ ���������� ����
            shader.uniform(name_shader_uniform[texture[i].type] + std::to_string(num_text[texture[i].type]), i);
            // � ��������� ��������
            texture[i].texture.use(GL_TEXTURE0 + i);// ����� ����������� ���������� ������ ���������� ����
        }
    }
   
    std::vector<Asset> texture;
    float specularFactor;
    glm::vec3 color;
protected: 
    bool texturable;
    void uniformMaterial(const Shader& shader) {
        shader.uniform("specularMaterial", specularFactor);
        if (texturable)bindAssets(shader);
        else shader.uniform("color", color);
    }
    void setMaterials(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gamma) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D specular;
        material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
        specularFactor = (specular.r + specular.b + specular.g) / 3.f;
        if (texturable && material->GetTextureCount(aiTextureType_DIFFUSE) == 0)  texturable = 0;

        if (texturable) {
            if (mesh->mMaterialIndex >= 0) addMaterial(material, directory, gamma);
        }
        else {
            aiColor4D Color;
            material->Get(AI_MATKEY_COLOR_DIFFUSE, Color);
            color = to_vec3(Color);
        }
    }
public:
    inline bool haveTexture() {
        return texturable;
    }
};
class VertexMesh:public MaterialMesh {
    VertexBufferObject VBO;
    ElementBufferObject EBO;
    // ���� �� ���� �������� ����
    void setVertex(aiMesh* mesh) {
        vertices.resize(mesh->mNumVertices);
        for (size_t i = 0; i < mesh->mNumVertices; i++)
        {

            // ����������
            vertices[i].position = to_vec3(mesh->mVertices[i]);
            // �������
            vertices[i].normal = to_vec3(mesh->mNormals[i]);
            // ���������� ����������
            if (texturable) {

                // ������� ����� ��������� �� 8 ��������� ���������� ���������. �� ������������, ��� �� ����� ������������ ������,
                // � ������� ������� ����� ��������� ��������� ���������� ���������, ������� �� ������ ����� ������ ����� (0)
                vertices[i].texCoord = to_vec2(mesh->mTextureCoords[0][i]);

            }
            else  vertices[i].texCoord = glm::vec2(0.f);
        }
    }
    // ���������� �� ������ ����� ���� (����� - ��� ����������� ����) � ��������� ��������������� ������� ������
    void setIndices(aiMesh* mesh) {

        indices.reserve(mesh->mNumFaces * 3);
        for (size_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            // �������� ��� ������� ������ � ��������� �� � ������� indices
            for (size_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
    }   
protected:
    // ������ ��� ���������� 
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    DrawBuffer VAO;
    VertexMesh() {}
    VertexMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gammaCorreciton) {
        texturable = mesh->mTextureCoords[0];
        std::cout << "Texturable:" << texturable << std::endl;
        setVertex(mesh);
        setIndices(mesh);
        setupVO();
        setMaterials(mesh, scene, directory, gammaCorreciton);
    }
    void setupVO()
    {
        // ������� �������� �������/�������
        VAO.begin();
        // ��������� ������ � ��������� �����
        VBO.begin();
        // ����� ������������� � ���������� ��, ��� ������������ � ������ �� ���������� ���������� �������� ����������������.
        // ����� ������� ����� � ���, ��� �� ����� ������ �������� ��������� �� ���������, � ��� ��������� ������������� � ������ ������ � ���������� ���� glm::vec3 (��� glm::vec2), ������� ����� ����� ������������ � ������ ������ float, �� � � ����� � � �������� ������
        VBO.data(vertices);

        EBO.begin();
        EBO.data(indices);

        // ������������� ��������� ��������� ���������
        VAO.attrib(0, 3, SIZE_VERTEX, 0); // ���������� ������
        VAO.attrib(1, 3, SIZE_VERTEX, offsetof(Vertex, normal)); // ������� ������
        VAO.attrib(2, 2, SIZE_VERTEX, offsetof(Vertex, texCoord));  // ���������� ���������� ������

        VAO.end();
    }
};
class Mesh :public VertexMesh, public Instancable {
    void init_data_draw() {
        id_obj = glShader::m_texture_normal;
        VAO.data_draw=DataDraw(DataDraw::DrawElements, GL_TRIANGLES, indices.size());
    }
    std::string name;
public:
    Mesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gammaCorreciton) :
        VertexMesh(mesh, scene, directory, gammaCorreciton) {
        name = mesh->mName.C_Str();
        init_data_draw();
    }
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
    void draw(View* view, const Shader& shader)
    {
        shader.use();
        view->use(shader);
        uniform(shader);
        uniformTransform(shader);
        VAO.draw();
    }
}; 
#endif