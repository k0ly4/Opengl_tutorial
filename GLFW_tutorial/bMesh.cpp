#include "bMesh.h"

/// <summary>
/// bVertex
/// </summary>
///
void bVertex::add(size_t id, float Weight) {

    for (size_t i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
        if (weight[i] == 0.f) {
            boneId[i] = id;
            weight[i] = Weight;
            return;
        }
    }
    size_t index = getMin(MAX_NUM_BONES_PER_VERTEX, weight);
    float factor = Weight;
    if (weight[index] < factor) {
        factor = weight[index];
        weight[index] = Weight;
        boneId[index] = id;
    }
}

void bVertex::distribute(float Weight) {
    Weight /= getSum(MAX_NUM_BONES_PER_VERTEX, weight);
    for (size_t i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) weight[i] += weight[i] * Weight;
}

void bVertex::normalizeWeight() {
    distribute(1.f - getSum(MAX_NUM_BONES_PER_VERTEX, weight));
}


/// <summary>
/// BoneMatrix
/// </summary>

void BoneMatrix::resize(size_t new_size) {
    offset.resize(new_size);
    matrix.resize(new_size);
    inverse_offset.resize(new_size);
}

void BoneMatrix::transform(size_t index, const glm::mat4& _matrix) {
    matrix[index] = _matrix * offset[index];
}

void BoneMatrix::push_back(const glm::mat4& _offset) {
    offset.push_back(_offset);
    matrix.push_back(glm::mat4(0.f));
    inverse_offset.push_back(glm::inverse(offset[offset.size() - 1]));
}

void BoneMatrix::push_back(const aiMatrix4x4& _offset) {
    offset.push_back(to_mat4(_offset));
    matrix.push_back(glm::mat4(0.f));
    inverse_offset.push_back(glm::inverse(offset[offset.size() - 1]));
}



/// <summary>
/// BonePointer
/// </summary>
///....


/// <summary>
/// Skeleton
/// </summary>

size_t Skeleton::get(aiBone* bone) {
    std::string b_name(bone->mName.C_Str());
    auto index = name_to_index.find(b_name);
    if (index == name_to_index.end()) {
        size_t bone_id = name_to_index.size();
        name_to_index[b_name] = bone_id;
        data.push_back(bone->mOffsetMatrix);
        return bone_id;
    }
    return index->second;
}



/// <summary>
/// NodeMatrixAnimation
/// </summary>
/// 
void NodeMatrixAnimation::interpolateVectorKey(const std::vector<VectorKey>& keys, glm::vec3& result) {
    if (keys.size() == 1)
        result = keys[0].value;
    else {
        size_t min_index = getIndex(keys);
        float factor = (*time_tick - keys[min_index].time) / (keys[min_index + 1].time - keys[min_index].time);
        assert(factor <= 1.f && factor >= 0.f);
        result = factor * (keys[min_index + 1].value - keys[min_index].value) + keys[min_index].value;
    }
}

void NodeMatrixAnimation::interpolateQuternionKey(const  std::vector<QuatKey>& keys, aiQuaternion& result) {
    if (keys.size() == 1)
        result = keys[0].value;
    else {
        size_t min_index = getIndex(keys);
        float factor = (*time_tick - keys[min_index].time) / (keys[min_index + 1].time - keys[min_index].time);
        assert(factor <= 1.f && factor >= 0.f);
        aiQuaternion::Interpolate(result, keys[min_index].value, keys[min_index + 1].value, factor);
        result.Normalize();
    }
}

glm::vec3 NodeMatrixAnimation::getPosition() {
    glm::vec3 result;
    interpolateVectorKey(pos, result);
    return result;
}

glm::vec3 NodeMatrixAnimation::getScale() {
    glm::vec3 result;
    interpolateVectorKey(scale, result);
    return result;
}

aiQuaternion NodeMatrixAnimation::getRotation() {
    aiQuaternion result;
    interpolateQuternionKey(rotate, result);
    return result;
}

glm::mat4 NodeMatrixAnimation::matScale() {
    glm::vec3 result;
    interpolateVectorKey(scale, result);
    return glm::scale(glm::mat4(1.f), result);
}

glm::mat4 NodeMatrixAnimation::matPosition() {
    glm::vec3 result;
    interpolateVectorKey(pos, result);
    return glm::translate(glm::mat4(1.f), result);
}

glm::mat4 NodeMatrixAnimation::matRotate() {
    aiQuaternion result;
    interpolateQuternionKey(rotate, result);
    return glm::mat4(to_mat3(result.GetMatrix()));
}

void NodeMatrixAnimation::create(const aiNodeAnim* node, float* time) {
    name = node->mNodeName.C_Str();
    time_tick = time;

    scale.resize(node->mNumScalingKeys);
    pos.resize(node->mNumPositionKeys);
    rotate.resize(node->mNumRotationKeys);

    for (size_t i = 0; i < scale.size(); i++)
        scale[i] = node->mScalingKeys[i];
    for (size_t i = 0; i < pos.size(); i++)
        pos[i] = node->mPositionKeys[i];
    for (size_t i = 0; i < rotate.size(); i++)
        rotate[i] = node->mRotationKeys[i];
}


/// <summary>
/// Animation
/// </summary>

void Animation::create(const aiAnimation* animation) {
    data.name = animation->mName.C_Str();
    data.duration = animation->mDuration;
    data.tickPerSecond = animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.f;
    channel.resize(animation->mNumChannels);
    size_t index = 0;
    for (auto i = channel.begin(); i != channel.end(); i++, index++) 
    {
        i->create(animation->mChannels[index], &data.time_pos);
    }
}

NodeMatrixAnimation* Animation::getChannel(const std::string& name) {
    for (auto i = channel.begin(); i != channel.end(); i++)
        if (i->name == name)return &(*i);
    return 0;
}

void Animation::tick(float time) {
    data.time_pos += time * data.tickPerSecond;
    if (data.time_pos > data.duration) {
        if (data.mode == repeat)  data.time_pos = 0.f;
        else  data.time_pos = data.duration;
    }
}
void Animation::printInfo() {
    data.print();
    for (auto i : channel)
        printf("        Channel:%s\n               scale:%d\n               pos:%d\n               rotate:%d\n",
            i.name.c_str(), i.scale.size(), i.pos.size(), i.rotate.size());
    printf("\n");
}

/// <summary>
/// AnimationPointer
/// </summary>

 void AnimationPointer::set(std::vector<Animation>& animations, const std::string& name_node, size_t* _cur_animation) {
     cur_animation = _cur_animation;
     channels.resize(animations.size());
     for (size_t i = 0; i < channels.size(); i++)
         channels[i] = animations[i].getChannel(name_node);
 }

/// <summary>
/// ManagerAnimation
/// </summary>

void ManagerAnimation::create(const aiScene* scene) {
    animation.resize(scene->mNumAnimations);
    for (size_t i = 0; i < scene->mNumAnimations; i++)
        animation[i].create(scene->mAnimations[i]);
}
void ManagerAnimation::printInfo() {
    printf("ManagerAnimation:\n     count animation:%d\n", animation.size());
    for (size_t i = 0; i < animation.size(); i++) {
        printf("    %s)", std::to_string(i).c_str());
        animation[i].printInfo();
    }
}

void ManagerAnimation::switchAnimation(size_t _cur_animation) {
    if (data.cur_animation != _cur_animation) {
        if (data.mode_switch == timePos_animation_restart) {
            animation[data.cur_animation].data.time_pos = 0.f;
        }
        data.cur_animation = _cur_animation;
    }
}

/// <summary>
/// bVertexMesh
/// </summary>
/// 

void bVertexMesh::setupVO()
{
    for (size_t i = 0; i < vertices.size(); i++)
        vertices[i].normalizeWeight();

    // Создаем буферные объекты/массивы
    VAO.begin();
    // Загружаем данные в вершинный буфер
    VBO.begin();
    // Самое замечательное в структурах то, что расположение в памяти их внутренних переменных является последовательным.
    // Смысл данного трюка в том, что мы можем просто передать указатель на структуру, и она прекрасно преобразуется в массив данных с элементами типа glm::vec3 (или glm::vec2), который затем будет преобразован в массив данных float, ну а в конце – в байтовый массив
    VBO.data(vertices);
    EBO.begin();
    EBO.data(indices);
    // Устанавливаем указатели вершинных атрибутов
    VAO.attrib(0, 3, SIZE_BONE_VERTEX, 0); // Координаты вершин
    VAO.attrib(1, 3, SIZE_BONE_VERTEX, offsetof(bVertex, normal)); // Нормали вершин
    VAO.attrib(2, 2, SIZE_BONE_VERTEX, offsetof(bVertex, texCoord));  // Текстурные координаты вершин

    VAO.attribI(3, 4, SIZE_BONE_VERTEX, offsetof(bVertex, boneId)); // Индентификаторы костей
    VAO.attrib(4, 4, SIZE_BONE_VERTEX, offsetof(bVertex, weight));  //Вес каждой кости по отношению к вершине

    VAO.end();

}

bVertexMesh::bVertexMesh(aiMesh* mesh, const aiScene* scene, Skeleton* bone_data, const std::string& directory, bool gammaCorreciton) {

    texturable = mesh->mTextureCoords[0];
    setVertex(mesh);
    setIndices(mesh);
    bones = bone_data;
    setBones(mesh);
    setupVO();
    setMaterials(mesh, scene, directory, gammaCorreciton);
}

// Цикл по всем вершинам меша
void bVertexMesh::setVertex(aiMesh* mesh) {
    vertices.resize(mesh->mNumVertices);
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        // Координаты
        vertices[i].position = to_vec3(mesh->mVertices[i]);
        // Нормали
        vertices[i].normal = to_vec3(mesh->mNormals[i]);
        // Текстурные координаты
        if (texturable) {
            // Вершина может содержать до 8 различных текстурных координат. Мы предполагаем, что не будем использовать модели,
            // в которых вершина может содержать несколько текстурных координат, поэтому мы всегда берем первый набор (0)
            vertices[i].texCoord = to_vec2(mesh->mTextureCoords[0][i]);

        }
        else  vertices[i].texCoord = glm::vec2(0.f);
    }
}
// Проходимся по каждой грани меша (грань - это треугольник меша) и извлекаем соответствующие индексы вершин
void bVertexMesh::setIndices(aiMesh* mesh) {

    indices.reserve(mesh->mNumFaces * 3);
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        // Получаем все индексы граней и сохраняем их в векторе indices
        for (size_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
}
//Кости
void bVertexMesh::setBones(aiMesh* mesh) {
    for (size_t i = 0; i < mesh->mNumBones; i++)
        setSingleBone(mesh->mBones[i]);
}

void bVertexMesh::setSingleBone(aiBone* bone) {
    int BoneId = bones->get(bone);
    for (size_t i = 0; i < bone->mNumWeights; i++) {
        const aiVertexWeight& vw = bone->mWeights[i];
        vertices[vw.mVertexId].add(BoneId, vw.mWeight);
    }
}

/// <summary>
/// bMesh
/// </summary>

    // Конструктор
bMesh::bMesh(aiMesh* mesh, const aiScene* scene, Skeleton* bone_data, const std::string& directory, bool gammaCorreciton) :
    bVertexMesh(mesh, scene, bone_data, directory, gammaCorreciton) {
    name = mesh->mName.C_Str();
    init_data_draw();
}
// Рендеринг меша
void bMesh::draw(const Shader& shader)
{
    uniform(shader);
    VAO.draw();
}

void bMesh::draw(const View* view, const Shader& shader)
{
    shader.use();
    view->use(shader);
    uniform(shader);
    uniformTransform(shader);
    VAO.draw();
}

void bMesh::init_data_draw() {
    id_obj = glShader::m_texture_normal;
    VAO.data_draw = DataDraw(DataDraw::DrawElements, GL_TRIANGLES, indices.size());
}

/// <summary>
/// NodeAnim
/// </summary>
/// 
NodeAnim::NodeAnim(const aiNode* node, const glm::mat4& parent_transform, Skeleton& bones, ManagerAnimation& _animation)
{
    name = node->mName.C_Str();
    aiMatrix4x4_to_mat4(node->mTransformation, default_transform);
    transform = default_transform;
    global = parent_transform * transform;
    bone = bones[name];
    _animation.getPointer(name, animation);
    if (!bone.isNull()) bone.transform(global);
}
void NodeAnim::tick(const glm::mat4& parent_transform, const glm::mat4& inverse_global) {
    if (!animation.isNull())  transform = animation.get().matTransform();
    else  transform = default_transform;
    global = parent_transform * transform;
    if (!bone.isNull()) bone.transform(inverse_global * global);
}