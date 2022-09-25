#ifndef MODEL_H
#define MODEL_H
#include "bMesh.h"
#include "Convex.h"
//структура для манипуляции мешами
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
class b_MeshArray {
protected:
    NodeAnim node;
    Skeleton bones;
    Convex skellet;
    ManagerAnimation animation;
    glm::mat4 inverse_global;
    glm::vec3 color_skellet = glm::vec3(0.f, 1.f, 0.f);
    void boneNode(NodeAnim& _node, Convex::Vertex*parent_vertex,size_t& global_index) {    
        if (_node.HasBone()) { 
            skellet[global_index] = { parent_vertex->pos, color_skellet };
            global_index++;
            skellet[global_index] = { glm::vec3(_node.getBone().matrix() * _node.getBone().inverse_offset() * glm::vec4(glm::vec3(0.f), 1.f)),
                color_skellet };
            parent_vertex = &skellet[global_index];
            global_index++;
        }
        for (size_t i = 0; i < _node.size(); i++)
            boneNode(_node[i], parent_vertex, global_index);
        
    }
    void tickNode(NodeAnim& _node, const glm::mat4& parent_transform) {
        _node.tick(parent_transform, inverse_global);
        for (size_t i = 0; i < _node.size(); i++) {
            tickNode(_node[i], _node.global);
        }
    }
    void infoNode(NodeAnim& _node, const std::string& tab) {
        printf("%sNode:%s, HaveBone:%d, HaveAnimation:%d, Childs:%d\n",
            tab.c_str(), _node.getName().c_str(), _node.HasBone(), _node.HasAnimationKeys(), _node.size());
        for (size_t i = 0; i < _node.size(); i++)
            infoNode(_node[i], tab + "----|");
    }
    friend class ModelLoader;
public:
    b_MeshArray() {
        skellet.setPrimitive(GlRender::LINES, 1, 10.f);
    }
    void tick(float time) {
        animation.tick(time);
        tickNode(node, glm::mat4(1.f));
    }
    ManagerAnimation& Animation() {

        return animation;
    }
    inline void push_back(const bMesh&& mesh) {
        meshes.push_back(mesh);
    }
    inline bMesh& operator[](size_t index) {
        return meshes[index];
    }
    Convex& getSkellet() {
        skellet.resize(bones.getMatrix().size()*2);
       size_t global_index =0;
        boneNode(node,&skellet[0], global_index);
        skellet[0] = skellet[1];    
        return skellet;
    }
protected:
    std::vector<bMesh> meshes;
};
class ModelLoader
{
    static const aiScene* scene;
    static Assimp::Importer importer;
    static bool gammaCorectiton;
    static std::string directory;

    // Рекурсивная обработка узла. Обрабатываем каждый отдельный меш, расположенный в узле, и повторяем этот процесс для дочерних узлов (если таковые вообще имеются)
    static void addNode(aiNode* node, MeshArray* meshes)
    {
        // Обрабатываем каждый меш текущего узла
        for (size_t i = 0; i < node->mNumMeshes; i++)
        {
            // Узел содержит только индексы объектов в сцене.
            // Сцена же содержит все данные; узел - это лишь способ организации данных
            meshes->push_back(Mesh(scene->mMeshes[node->mMeshes[i]], scene, directory, gammaCorectiton));
        }
        // После того, как мы обработали все меши (если таковые имелись), мы начинаем рекурсивно обрабатывать каждый из дочерних узлов
        for (size_t i = 0; i < node->mNumChildren; i++)
            addNode(node->mChildren[i], meshes);
    }
    static void addNode(aiNode* node, b_MeshArray* meshes)
    {
        // Обрабатываем каждый меш текущего узла
        for (size_t i = 0; i < node->mNumMeshes; i++)
        {
            // Узел содержит только индексы объектов в сцене.
            // Сцена же содержит все данные; узел - это лишь способ организации данных
            meshes->push_back(bMesh(scene->mMeshes[node->mMeshes[i]], scene, &meshes->bones, directory, gammaCorectiton));
        }
        // После того, как мы обработали все меши (если таковые имелись), мы начинаем рекурсивно обрабатывать каждый из дочерних узлов
        for (size_t i = 0; i < node->mNumChildren; i++)
            addNode(node->mChildren[i], meshes);
    }
    static void addNode(aiNode* aiNode, b_MeshArray* meshes, NodeAnim& node)
    {    
        for (size_t i = 0; i < aiNode->mNumChildren; i++)     
            addNode(aiNode->mChildren[i], meshes, node.push_back(NodeAnim(aiNode->mChildren[i], node.global, meshes->bones, meshes->animation)));
    }
    static void parse_bones(const aiMesh* mesh) {
        for (size_t i = 0; i < mesh->mNumBones; i++)
        {
            aiBone* bone = mesh->mBones[i];
            printf("            %s\n", bone->mName.C_Str());
        }
    }
    static void parse_node(aiNode* node, glm::ivec3& total) {
        printf("Node: %s\n", node->mName.C_Str());
        for (size_t i = 0; i < node->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            int n_vert = mesh->mNumVertices;
            int n_ind = mesh->mNumFaces * 3;
            int n_bone = mesh->mNumBones;
            printf("    %d.Mesh:\n          name: %s\n          vertices: %d\n          indices: %d\n          bones: %d\n",
                i, mesh->mName.C_Str(), n_vert, n_ind, n_bone);
            total += glm::vec3(n_vert, n_ind, n_bone);
            if (mesh->HasBones()) {
                parse_bones(mesh);
            }
        }
        for (size_t i = 0; i < node->mNumChildren; i++)
            parse_node(node->mChildren[i], total);
    }

    static bool read(const std::string& path) {
        // Чтение файла с помощью Assimp
        scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        // Проверка на ошибки
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // если НЕ 0
        {
            std::cout << "ERROR::ASSIMP::(path == " << path << ")::" << importer.GetErrorString() << std::endl;
            return 0;
        }

        // Получение пути к файлу
        directory = path.substr(0, path.find_last_of('/'));
        return 1;
    }
public:

   static bool load(const std::string& path, MeshArray* meshes, bool gamma, bool infoMode)
    {
       if (!read(path))return 0;
        gammaCorectiton = gamma;
        // Рекурсивная обработка корневого узла Assimp
        addNode(scene->mRootNode,meshes);
        if (infoMode) printInfo({});
        importer.FreeScene();
        return 1;
    }
    static bool printInfo(const std::string& path) {
        if (!path.empty()) 
            if (!read(path))
                return 0;
        
        printf("\nLoading model from %s\n", directory.c_str());
        glm::ivec3 total(0);
        parse_node(scene->mRootNode, total);
        printf("TOTAL: meshes: %d, vertices: %d, indices: %d, bones: %d\n\n",
            scene->mNumMeshes, total.x, total.y, total.z);

        if (!path.empty())importer.FreeScene();
        return 1;
    }
    static bool load(const std::string& path, b_MeshArray* meshes, bool gamma, bool infoMode)
    {
        if (!read(path))return 0;
        gammaCorectiton = gamma;
        aiMatrix4x4 global = scene->mRootNode->mTransformation;
        print(global);
        global.Inverse();
        aiMatrix4x4_to_mat4(global, meshes->inverse_global);

        if (scene->mNumAnimations) 
            meshes->animation.create(scene);

        addNode(scene->mRootNode, meshes);
        meshes->node = NodeAnim(scene->mRootNode, glm::mat4(1.f), meshes->bones, meshes->animation);
        addNode(scene->mRootNode, meshes, meshes->node);
        if (infoMode)
            printInfo({});
        importer.FreeScene();
        return 1;
    }
    private:
        ModelLoader() {}
        ~ModelLoader() {}
};
const aiScene* ModelLoader::scene;
Assimp::Importer ModelLoader::importer;
bool ModelLoader::gammaCorectiton;
std::string ModelLoader::directory;

class Model:public MeshArray,public Drawable,public Transformable3D {
public:
    inline size_t size()const{
        return meshes.size();
    }
    // Отрисовываем модель, а значит и все её меши
    inline void load(const std::string& path, bool gamma = 0, bool info = 0) {
        ModelLoader::load(path, this, gamma, info);
        if (meshes[0].haveTexture()) id_obj = glShader::gb_texturable;
        else id_obj = glShader::gb_color_uniform;
      
    }
    void draw(View* view, const Shader& shader) {
        shader.use();
        uniformTransform(shader);
        view->use(shader);
        for (size_t i = 0; i < meshes.size(); i++)
            meshes[i].draw(shader);
    }
};
class InstanceModel :public Drawable, public Transformable3D {
    std::vector<InstanceObject> target;
    VertexBufferObject m_VBO;
    std::vector<glm::mat4> matrix;
    //Интерпритация матриц
public:
    inline size_t size() {
        return target.size();
    }
    virtual void setObject(Model& _model, glShader::Object shader_configuraion) {
        setTransform(_model.getTransform());
        target.resize(_model.size());
        for (size_t i = 0; i < _model.size(); i++)
            target[i].setObject(&_model[i], shader_configuraion);
        id_obj = shader_configuraion;
    }
    inline virtual void draw(View* view, const Shader& shader) {
        for (size_t i = 0; i < target.size(); i++)
            target[i].draw(view, shader);
    }
    virtual void create(const std::vector<glm::mat4>& matrix) {
        this->matrix = matrix;
        std::cout << matrix.size();
        ArrayBufferObject::end();
        m_VBO.begin();
        m_VBO.setMode(GL_DYNAMIC_DRAW);
        m_VBO.data(matrix);

        for (size_t i = 0; i < target.size(); i++)
            target[i].create(m_VBO,matrix.size());
        ArrayBufferObject::end();
        m_VBO.end();
    }
};

class b_Model :public b_MeshArray, public Drawable, public Transformable3D {
    int boneMode = 0;
    int boneIndex = 0;
public:
    inline size_t size()const {
        return meshes.size();
    }
    inline void load(const std::string& path, bool gamma = 0,bool info =0) {
        ModelLoader::load(path, this, gamma, info);
        if (meshes[0].haveTexture()) id_obj = glShader::gb_texture_animation;
        else id_obj = glShader::gb_color_uniform_animation;
        infoNode(node, "");
    }
    void setBoneMode(bool mode) {
        boneMode = mode;
        std::cout << "SetBoneMode:" << mode << "\n";
    }
    void setBoneID(int id) {
        std::cout << "SetBoneId:" << id << '\n';
        boneIndex = id;
    }
    void draw(View* view, const Shader& shader) {
        shader.use();
        uniformTransform(shader);
        shader.uniform("boneMode", boneMode);
        shader.uniform("gDisplayBoneIndex", boneIndex);
        shader.uniform("bones",bones.getMatrix());
        view->use(shader);
        for (size_t i = 0; i < meshes.size(); i++) meshes[i].draw(shader);
    }
};

#endif
