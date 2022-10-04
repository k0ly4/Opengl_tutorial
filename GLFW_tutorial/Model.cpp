#include "Model.h"
//структура для манипуляции мешами

/// <summary>
/// bMeshArray
/// </summary>

void bMeshArray::tick(float time) {
    animation.tick(time);
    tickNode(node, glm::mat4(1.f));
}

Convex& bMeshArray::getSkellet() {
    skellet.resize(bones.getMatrix().size() * 2);
    size_t global_index = 0;
    boneNode(node, &skellet[0], global_index);
    skellet[0] = skellet[1];
    return skellet;
}

void bMeshArray::boneNode(NodeAnim& _node, ConvexVertex* parent_vertex, size_t& global_index) {
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

void bMeshArray::tickNode(NodeAnim& _node, const glm::mat4& parent_transform) {
    _node.tick(parent_transform, inverse_global);
    for (size_t i = 0; i < _node.size(); i++) {
        tickNode(_node[i], _node.global);
    }
}

void bMeshArray::infoNode(NodeAnim& _node, const std::string& tab) {
    printf("%sNode:%s, HaveBone:%d, HaveAnimation:%d, Childs:%d\n",
        tab.c_str(), _node.getName().c_str(), _node.HasBone(), _node.HasAnimationKeys(), _node.size());
    for (size_t i = 0; i < _node.size(); i++)
        infoNode(_node[i], tab + "----|");
}

/// <summary>
/// ModelLoader
/// </summary>

 bool ModelLoader::load(const std::string& path, MeshArray* meshes, bool gamma, bool infoMode)
{
    if (!read(path))return 0;
    gammaCorectiton = gamma;
    // Рекурсивная обработка корневого узла Assimp
    addNode(scene->mRootNode, meshes);
    if (infoMode) printInfo({});
    importer.FreeScene();
    return 1;
}

 bool ModelLoader::printInfo(const std::string& path) {
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

 bool ModelLoader::load(const std::string& path, bMeshArray* meshes, bool gamma, bool infoMode)
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
// Рекурсивная обработка узла. Обрабатываем каждый отдельный меш, расположенный в узле, и повторяем этот процесс для дочерних узлов (если таковые вообще имеются)
 void ModelLoader::addNode(aiNode* node, MeshArray* meshes)
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

 void ModelLoader::addNode(aiNode* node, bMeshArray* meshes)
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

 void ModelLoader::addNode(aiNode* aiNode, bMeshArray* meshes, NodeAnim& node)
{
    for (size_t i = 0; i < aiNode->mNumChildren; i++)
        addNode(aiNode->mChildren[i], meshes, node.push_back(NodeAnim(aiNode->mChildren[i], node.global, meshes->bones, meshes->animation)));
}

 void ModelLoader::parse_bones(const aiMesh* mesh) {
    for (size_t i = 0; i < mesh->mNumBones; i++)
    {
        aiBone* bone = mesh->mBones[i];
        printf("            %s\n", bone->mName.C_Str());
    }
}

 void ModelLoader::parse_node(aiNode* node, glm::ivec3& total) {
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

 bool ModelLoader::read(const std::string& path) {
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

const aiScene* ModelLoader::scene;
Assimp::Importer ModelLoader::importer;
bool ModelLoader::gammaCorectiton;
std::string ModelLoader::directory;

/// <summary>
/// Model
/// </summary>
/// 
// Отрисовываем модель, а значит и все её меши
void Model::load(const std::string& path, bool gamma, bool info) {
    ModelLoader::load(path, this, gamma, info);
    if (meshes[0].haveTexture()) id_obj = glShader::gb_texturable;
    else id_obj = glShader::gb_color_uniform;
}
void Model::draw(View* view, const Shader& shader) {
    shader.use();
    uniformTransform(shader);
    view->use(shader);
    for (size_t i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);
}
/// <summary>
/// InstanceModel
/// </summary>

void InstanceModel::setObject(Model& _model, glShader::Object shader_configuraion) {
    setTransform(_model.getTransform());
    target.resize(_model.size());
    for (size_t i = 0; i < _model.size(); i++)
        target[i].setObject(&_model[i], shader_configuraion);
    id_obj = shader_configuraion;
}

void InstanceModel::draw(View* view, const Shader& shader) {
    for (size_t i = 0; i < target.size(); i++)
        target[i].draw(view, shader);
}

void InstanceModel::create(const std::vector<glm::mat4>& matrix) {
    this->matrix = matrix;
    std::cout << matrix.size();
    ArrayBufferObject::end();
    m_VBO.begin();
    m_VBO.setMode(GL_DYNAMIC_DRAW);
    m_VBO.data(matrix);

    for (size_t i = 0; i < target.size(); i++)
        target[i].create(m_VBO, matrix.size());
    ArrayBufferObject::end();
    m_VBO.end();
}

/// <summary>
/// bModel
/// </summary>
/// 
void bModel::load(const std::string& path, bool gamma, bool info) {
    ModelLoader::load(path, this, gamma, info);
    if (meshes[0].haveTexture()) id_obj = glShader::gb_texture_animation;
    else id_obj = glShader::gb_color_uniform_animation;
    infoNode(node, "");
}

void bModel::draw(View* view, const Shader& shader) {
    shader.use();
    uniformTransform(shader);
    shader.uniform("boneMode", boneMode);
    shader.uniform("gDisplayBoneIndex", boneIndex);
    shader.uniform("bones", bones.getMatrix());
    view->use(shader);
    for (size_t i = 0; i < meshes.size(); i++) meshes[i].draw(shader);
}
