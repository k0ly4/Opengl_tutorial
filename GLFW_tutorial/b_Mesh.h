#ifndef B_MESH_H
#define B_MESH_H
#include "Mesh.h"
template<typename type>
size_t Min(size_t size,const type* data) {
    size_t result =0;
    for(size_t i=0;i<size-1;i++)
        if (data[result] > data[i])result = i;
   
    return result;
}
template<typename type>
type Sum(size_t size, const type* data) {
    type result = data[0];
    for (size_t i = 1; i < size; i++) 
       result+= data[i];
    return result;
}

struct b_Vertex {
#define MAX_NUM_BONES_PER_VERTEX 4
    // Позиция
    glm::vec3 position;
    //12u
    // Нормаль
    glm::vec3 normal;
    //24u
    // Текстурные координаты
    glm::vec2 texCoord;
    ///32u
    size_t boneId[MAX_NUM_BONES_PER_VERTEX] = { 0 };
    //48u
    float weight[MAX_NUM_BONES_PER_VERTEX] = { 0.f };
    //64u
    void add(size_t id, float Weight) {
       
            for (size_t i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
                if (weight[i] == 0.f) {
                    boneId[i] = id;
                    weight[i] = Weight;
                    return;
                }
            }
            size_t index = Min(MAX_NUM_BONES_PER_VERTEX, weight);
            float factor = Weight;
            if (weight[index] < factor) {
                factor = weight[index];
                weight[index] = Weight;
                boneId[index] = id;
            }
    }
    void distribute(float Weight) {
        Weight /= Sum(MAX_NUM_BONES_PER_VERTEX, weight);
        for (size_t i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) weight[i] += weight[i] * Weight;
    }
    void normalizeWeight() {
       distribute(1.f - Sum(MAX_NUM_BONES_PER_VERTEX, weight));       
    }
};
#define B_SIZE_VERTEX sizeof(b_Vertex)
struct BoneMatrix {
    std::vector<glm::mat4>inverse_offset;
    std::vector<glm::mat4>offset;
    std::vector<glm::mat4> matrix;

    void resize(size_t new_size) {
        offset.resize(new_size);
        matrix.resize(new_size);
        inverse_offset.resize(new_size);
    }
    void transform(size_t index, const glm::mat4& _matrix) {
        matrix[index] = _matrix * offset[index];
    }
    void push_back(const glm::mat4& _offset) {
        offset.push_back(_offset);
        matrix.push_back(glm::mat4(0.f));
        inverse_offset.push_back(glm::inverse(offset[offset.size() - 1]));
    }
    void push_back(const aiMatrix4x4& _offset) {
        offset.push_back(to_mat4(_offset));
        matrix.push_back(glm::mat4(0.f));
        inverse_offset.push_back(glm::inverse(offset[offset.size()-1]));
    }
};
class BonePointer {
    BoneMatrix* bones;
    size_t index;
public:
    BonePointer(BoneMatrix* _bones, size_t _index) :bones(_bones), index(_index) {}
    BonePointer() :bones(0) {}
    bool isNull()const  {
        return bones == 0;
    }
    void transform(const glm::mat4& _matrix) {
        bones->transform(index, _matrix);
    }
    glm::mat4& matrix() {
        return bones->matrix[index];
    }
    glm::mat4& offset()const {
        return bones->offset[index];
    }

    glm::mat4& matrix()const {
        return bones->matrix[index];
    }
    glm::mat4& inverse_offset()const {
        return bones->inverse_offset[index];
    }
    
};
class Skeleton {
    BoneMatrix data;
    std::map<std::string, size_t> name_to_index;
public:
    const std::vector<glm::mat4>& getMatrix() {
        return data.matrix;
    }
    inline BonePointer operator [](const std::string& name) {
        auto index = name_to_index.find(name);
        if (index != name_to_index.end()) return BonePointer(&data, index->second);
        return BonePointer();
    }
    size_t get(aiBone* bone) {
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
};

class NodeMatrixAnimation {
    struct VectorKey {
        glm::vec3 value;
        float time;
        const VectorKey& operator =(const aiVectorKey& vecKey) {
            value = to_vec3(vecKey.mValue);
            time = vecKey.mTime;
            return *this;
        }
    };
    struct QuatKey {
        aiQuaternion value;
        float time;
        const QuatKey& operator =(const aiQuatKey& Key) {
            value = Key.mValue;
            time = Key.mTime;
            return *this;
        }
    };

    template<typename key>
    size_t getIndex(const std::vector<key>& keys) {
        for (size_t i = 0; i < keys.size() - 1; i++)
            if (keys[i + 1].time > *time_tick)
                return i;
        return 0;
    }
    void interpolateVectorKey(const std::vector<VectorKey>& keys, glm::vec3& result) {
        if (keys.size() == 1)
            result = keys[0].value;
        else {
            size_t min_index = getIndex(keys);
            float factor = (*time_tick - keys[min_index].time) / (keys[min_index + 1].time - keys[min_index].time);
            assert(factor <= 1.f && factor >= 0.f);
            result = factor * (keys[min_index + 1].value - keys[min_index].value) + keys[min_index].value;
        }
    }
    void interpolateQuternionKey(const  std::vector<QuatKey>& keys, aiQuaternion& result) {
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
    float* time_tick;
public: 
      std::vector<VectorKey> scale;
      std::vector<VectorKey> pos;
      std::vector<QuatKey> rotate;
      std::string name;

      glm::vec3 getPosition() {
          glm::vec3 result;
          interpolateVectorKey(pos, result);
          return result;
      }
      glm::vec3 getScale() {
          glm::vec3 result;
          interpolateVectorKey(scale, result);
          return result;
      }
      aiQuaternion getRotation() {
          aiQuaternion result;
          interpolateQuternionKey(rotate, result);
          return result;
      }

      glm::mat4 matScale() {
          glm::vec3 result;
          interpolateVectorKey(scale, result);
          return glm::scale(glm::mat4(1.f), result);
      }
      glm::mat4 matPosition() {
          glm::vec3 result;
          interpolateVectorKey(pos, result);
          return glm::translate(glm::mat4(1.f), result);
      }
      glm::mat4 matRotate() {
          aiQuaternion result;
          interpolateQuternionKey(rotate, result);
          return glm::mat4(to_mat3(result.GetMatrix()));
      }
      glm::mat4 matTransform() {
          return matPosition() * matRotate() * matScale();
      }

      void create(const aiNodeAnim* node, float* time) {
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
};
class Animation {
    std::list<NodeMatrixAnimation>channel;
    friend class ManagerAnimation;
    enum Behavior
    {
        repeat, non_repeat
    };
    struct Setup {
        std::string name;
        float duration;
        float tickPerSecond;
        float time_pos;
        Behavior mode = repeat;
        void print() {
            printf("Animation:%s\n           duration:%f\n           tickPerSecond:%f\n", name.c_str(), duration, tickPerSecond);
        }
    }data;
public:
    void create(const aiAnimation* animation) {
        data.name = animation->mName.C_Str();
        data.duration = animation->mDuration;
        data.tickPerSecond = animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.f;
        channel.resize(animation->mNumChannels);
        size_t index = 0;
        for (auto i = channel.begin(); i != channel.end(); i++, index++)
            i->create(animation->mChannels[index], &data.time_pos);
    }
    NodeMatrixAnimation* getChannel(const std::string& name) {
        for (auto i = channel.begin(); i != channel.end(); i++)
            if (i->name == name)return &(*i);
        return 0;
    }
    const Setup& getSetup() {
        return data;
    }
    void setMode(Behavior _mode) {
        data.mode = _mode;
    }
    void setTickPerSecond(float value) {
        data.tickPerSecond = value;
    }
    void tick(float time) {
        data.time_pos += time * data.tickPerSecond;
        if (data.time_pos > data.duration) {
            if (data.mode == repeat)  data.time_pos = 0.f;
            else  data.time_pos = data.duration;
        }
    }
    void printInfo() {
        data.print();
        for (auto i : channel)
            printf("        Channel:%s\n               scale:%d\n               pos:%d\n               rotate:%d\n",
                i.name.c_str(), i.scale.size(), i.pos.size(), i.rotate.size());
        printf("\n");
    }
};
class AnimationPointer {
    std::vector<NodeMatrixAnimation*>channels;
    size_t* cur_animation;
public:
    AnimationPointer() : cur_animation(0) {}
    inline void set(std::vector<Animation>& animations, const std::string& name_node, size_t* _cur_animation) {
        cur_animation = _cur_animation;
        channels.resize(animations.size());
        for (size_t i = 0; i < channels.size(); i++)
            channels[i] = animations[i].getChannel(name_node);
    }
    inline bool isNull() {
        return channels[*cur_animation] == 0;
    }
    inline NodeMatrixAnimation& get() {
        return *channels[*cur_animation];
    }
};
class ManagerAnimation {
    std::vector<Animation> animation;
    std::map<std::string, size_t>name_to_index; 
    enum Switch
    {
        timePos_animation_restart, timePos_animation_save
    };
    struct Setup {
        Switch mode_switch = timePos_animation_restart;
        size_t cur_animation;
    }data;
public:
    void setModeSwitch(Switch state) {
        data.mode_switch = state;
    }
    void create(const aiScene* scene) {
        animation.resize(scene->mNumAnimations);
        for (size_t i = 0; i < scene->mNumAnimations; i++)
            animation[i].create(scene->mAnimations[i]);
    }
    void printInfo() {
        printf("ManagerAnimation:\n     count animation:%d\n", animation.size());
        for (size_t i = 0; i < animation.size(); i++) {
            printf("    %s)", std::to_string(i).c_str());
            animation[i].printInfo();
        }
    }
    inline void switchAnimation(const std::string& name) {
        switchAnimation(name_to_index[name]);
    }
    void switchAnimation(size_t _cur_animation) {
        if (data.cur_animation != _cur_animation) {
            if (data.mode_switch == timePos_animation_restart) {
                animation[data.cur_animation].data.time_pos = 0.f;
            }
            data.cur_animation = _cur_animation;
        }
    }
    void tick(float time) {
        animation[data.cur_animation].tick(time);
    }
    void getPointer(const std::string& name,AnimationPointer& pointer) {
        pointer.set(animation, name, &data.cur_animation);
    }
    const Setup& getSetup() {
        return data;
    }

    size_t size() {
        return animation.size();
    }
    Animation& operator [](const std::string& name) {
        return animation[name_to_index[name]];
    }
    Animation& operator [](size_t index) {
        return animation[index];
    }
};

class b_VertexMesh :public MaterialMesh {
    // Данные меша
    VertexBufferObject VBO;
    ElementBufferObject EBO;
    // Цикл по всем вершинам меша
    void setVertex(aiMesh* mesh) {
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
    void setIndices(aiMesh* mesh) {

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
    void setBones(aiMesh* mesh) {
        for (size_t i = 0; i < mesh->mNumBones; i++)
            setSingleBone(mesh->mBones[i]);
    }
    void setSingleBone(aiBone* bone) {
        int BoneId = bones->get(bone);
        for (size_t i = 0; i < bone->mNumWeights; i++) {
            const aiVertexWeight& vw = bone->mWeights[i];
            vertices[vw.mVertexId].add(BoneId, vw.mWeight);
        }
    }
protected:
    // Данные для рендеринга 
    DrawBuffer VAO;
    std::vector<b_Vertex> vertices;
    std::vector<unsigned int> indices;
    Skeleton* bones;
    // Инициализируем все буферные объекты/массивы
    void setupVO()
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
        VAO.attrib(0, 3, B_SIZE_VERTEX, 0); // Координаты вершин
        VAO.attrib(1, 3, B_SIZE_VERTEX, offsetof(b_Vertex, normal)); // Нормали вершин
        VAO.attrib(2, 2, B_SIZE_VERTEX, offsetof(b_Vertex, texCoord));  // Текстурные координаты вершин

        VAO.attribI(3, 4, B_SIZE_VERTEX, offsetof(b_Vertex, boneId)); // Индентификаторы костей
        VAO.attrib(4, 4, B_SIZE_VERTEX, offsetof(b_Vertex, weight));  //Вес каждой кости по отношению к вершине

        VAO.end();
        
    }
    b_VertexMesh() {}
    b_VertexMesh(aiMesh* mesh, const aiScene* scene, Skeleton* bone_data, const std::string& directory, bool gammaCorreciton) {

        texturable = mesh->mTextureCoords[0];
        setVertex(mesh);
        setIndices(mesh);
        bones = bone_data;
        setBones(mesh);
        setupVO();
        setMaterials(mesh, scene, directory, gammaCorreciton);
    }
};
class b_Mesh :public b_VertexMesh, public Instancable {
    void init_data_draw() {
        id_obj = glShader::m_texture_normal;
        VAO.data_draw = DataDraw(DataDraw::DrawElements, GL_TRIANGLES, indices.size());
    }
    std::string name;
public:
    // Конструктор

    b_Mesh(aiMesh* mesh, const aiScene* scene, Skeleton* bone_data, const std::string& directory, bool gammaCorreciton) :
        b_VertexMesh(mesh, scene, bone_data, directory, gammaCorreciton) {
        name = mesh->mName.C_Str();
        init_data_draw();
    }
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
        VAO.draw();
    }
    void draw(View* view, const Shader& shader)
    {
        shader.use();
        view->use(shader);
        uniform(shader);
        Transformable3D::uniform(shader);
        VAO.draw();
    }
};
class NodeAnim {
    glm::mat4 default_transform;
    std::vector<NodeAnim> child;
    BonePointer bone;
    AnimationPointer animation;
    std::string name;
public:
    glm::mat4 transform;
    glm::mat4 global;

    NodeAnim() :default_transform(1.f), transform(1.f), global(1.f) {}
    NodeAnim(const aiNode* node, const glm::mat4& parent_transform, Skeleton& bones, ManagerAnimation& _animation)
    {
        name = node->mName.C_Str();
        aiMatrix4x4_to_mat4(node->mTransformation, default_transform);
        transform = default_transform;
        global = parent_transform * transform;

        bone = bones[name];
        _animation.getPointer(name, animation);
        if (!bone.isNull()) bone.transform(global);
    }
    const std::string& getName() {
        return name;
    }
    void bind(Skeleton& bones) {
        bone = bones[name];
    }
    void bind(ManagerAnimation& _animation) {
        _animation.getPointer(name, animation);
    }
    void tick(const glm::mat4& parent_transform, const glm::mat4& inverse_global) {
        if (!animation.isNull())  transform = animation.get().matTransform();
        else  transform = default_transform;
        global = parent_transform * transform;
        if (!bone.isNull()) bone.transform(inverse_global * global);
    }
    bool HasBone() {
        return !bone.isNull();
    }
    bool HasAnimationKeys() {
        return !animation.isNull();
    }
    const BonePointer& getBone() {
        return bone;
    }
    //childs
    size_t size() {
        return child.size();
    }
    NodeAnim& operator[](size_t index) {
        return child[index];
    }
    NodeAnim& push_back(NodeAnim&& node) {
        child.push_back(node);
        return child[child.size()-1];
    }
};
#endif