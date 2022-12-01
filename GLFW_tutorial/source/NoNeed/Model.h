#ifndef MODEL_H
#define MODEL_H
#include "Resource/bMesh.h"
#include "Scene/Convex.h"
//структура дл€ манипул€ции мешами

/// <summary>
/// MeshArray
/// </summary>
class MeshArray{
public:
    inline void push_back(const Mesh&& mesh) {
        meshes.push_back(mesh);
    }
    inline Mesh& operator[](size_t index) {
        return meshes[index];
    }
protected:
    std::vector<Mesh> meshes;
};
/// <summary>
/// bMeshArray
/// </summary>
class bMeshArray {

public:

    bMeshArray() {
        skellet.setPrimitive(GlRender::LINES, 1, 10.f);
    }

    void tick(float time);

    ManagerAnimation& Animation() {

        return animation;
    }

    inline void push_back(const bMesh&& mesh) {
        meshes.push_back(mesh);
    }

    inline bMesh& operator[](size_t index) {
        return meshes[index];
    }

    Convex& getSkellet();

protected:

    NodeAnim node;
    Skeleton bones;
    Convex skellet;
    ManagerAnimation animation;
    std::vector<bMesh> meshes;

    glm::mat4 inverse_global;
    glm::vec3 color_skellet = glm::vec3(0.f, 1.f, 0.f);
    void boneNode(NodeAnim& _node, ConvexVertex* parent_vertex, size_t& global_index);
    void tickNode(NodeAnim& _node, const glm::mat4& parent_transform);
    void infoNode(NodeAnim& _node, const std::string& tab);
    friend class ModelLoader;
  
};

/// <summary>
/// ModelLoader
/// </summary>
class ModelLoader
{
  
public:

    static bool load(const std::string& path, MeshArray* meshes, bool gamma, bool infoMode);

   static bool printInfo(const std::string& path);

   static bool load(const std::string& path, bMeshArray* meshes, bool gamma, bool infoMode);

    private:

        static const aiScene* scene;
        static Assimp::Importer importer;
        static bool gammaCorectiton;
        static std::string directory;

        ModelLoader() {}
        ~ModelLoader() {}
        // –екурсивна€ обработка узла. ќбрабатываем каждый отдельный меш, расположенный в узле, и повтор€ем этот процесс дл€ дочерних узлов (если таковые вообще имеютс€)
        static void addNode(aiNode* node, MeshArray* meshes);

        static void addNode(aiNode* node, bMeshArray* meshes);

        static void addNode(aiNode* aiNode, bMeshArray* meshes, NodeAnim& node);

        static void parse_bones(const aiMesh* mesh);

        static void parse_node(aiNode* node, glm::ivec3& total);

        static bool read(const std::string& path);
};

/// <summary>
/// Model
/// </summary>
class Model:public MeshArray,public Drawable,public Transformable3D {

public:

    inline size_t size()const{
        return meshes.size();
    }
    // ќтрисовываем модель, а значит и все еЄ меши
    inline void load(const std::string& path, bool gamma = 0, bool info = 0);

    void draw(View* view, const Shader& shader);
};

/// <summary>
/// bModel
/// </summary>
/// 
class bModel :public bMeshArray, public Drawable, public Transformable3D {

public:

    inline size_t size()const {
        return meshes.size();
    }

    void load(const std::string& path, bool gamma = 0, bool info = 0);

    void setBoneMode(bool mode) {
        boneMode = mode;
        std::cout << "SetBoneMode:" << mode << "\n";
    }

    void setBoneID(int id) {
        std::cout << "SetBoneId:" << id << '\n';
        boneIndex = id;
    }

    void draw(View* view, const Shader& shader);

private:
    int boneMode = 0;
    int boneIndex = 0;

};

#endif
