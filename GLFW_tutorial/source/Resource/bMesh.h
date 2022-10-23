#ifndef BONE_MESH_H
#define BONE_MESH_H

#include "Graphic/Mesh.h"

/// <summary>
/// bVertex
/// </summary>
struct bVertex {
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
    void add(size_t id, float Weight);
    void distribute(float Weight);
    void normalizeWeight();
};
#define SIZE_BONE_VERTEX sizeof(bVertex)

/// <summary>
/// BoneMatrix
/// </summary>
class BoneMatrix {

public:

    std::vector<glm::mat4>inverse_offset;
    std::vector<glm::mat4>offset;
    std::vector<glm::mat4> matrix;

    void resize(size_t new_size);
    void transform(size_t index, const glm::mat4& _matrix);
    void push_back(const glm::mat4& _offset);
    void push_back(const aiMatrix4x4& _offset);
};

/// <summary>
/// BonePointer
/// </summary>
class BonePointer {
   
public:
    BonePointer(BoneMatrix* _bones, size_t _index) :bones(_bones), index(_index) {}
    BonePointer() :bones(0) {}
    bool isNull()const {
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

private:
    BoneMatrix* bones;
    size_t index;
};

/// <summary>
/// Skeleton
/// </summary>
//
class Skeleton {

public:

    const std::vector<glm::mat4>& getMatrix() {
        return data.matrix;
    }

    inline BonePointer operator [](const std::string& name) {
        auto index = name_to_index.find(name);
        if (index != name_to_index.end()) return BonePointer(&data, index->second);
        return BonePointer();
    }

    size_t get(aiBone* bone);

private:
    BoneMatrix data;
    std::map<std::string, size_t> name_to_index;
};

/// <summary>
/// NodeMatrixAnimation
/// </summary>
class NodeMatrixAnimation {

    struct VectorKey {
        glm::vec3 value;
        float time;
        const VectorKey& operator =(const aiVectorKey& vecKey) {
            value = to_vec3(vecKey.mValue);
            time = (float)vecKey.mTime;
            return *this;
        }
    };

    struct QuatKey {
        aiQuaternion value;
        float time;
        const QuatKey& operator =(const aiQuatKey& Key) {
            value = Key.mValue;
            time = (float)Key.mTime;
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

    void interpolateVectorKey(const std::vector<VectorKey>& keys, glm::vec3& result);

    void interpolateQuternionKey(const  std::vector<QuatKey>& keys, aiQuaternion& result);

    float* time_tick;

public:
    std::vector<VectorKey> scale;
    std::vector<VectorKey> pos;
    std::vector<QuatKey> rotate;
    std::string name;

    glm::vec3 getPosition();
    glm::vec3 getScale();
    aiQuaternion getRotation();

    glm::mat4 matScale();
    glm::mat4 matPosition();
    glm::mat4 matRotate();
    glm::mat4 matTransform() {
        return matPosition() * matRotate() * matScale();
    }

    void create(const aiNodeAnim* node, float* time);
};

/// <summary>
/// Animation
/// </summary>
class Animation {

public:

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
    };

    void create(const aiAnimation* animation);
    NodeMatrixAnimation* getChannel(const std::string& name);

    const Setup& getSetup() {
        return data;
    }
    void setMode(Behavior _mode) {
        data.mode = _mode;
    }
    void setTickPerSecond(float value) {
        data.tickPerSecond = value;
    }

    void tick(float time);
    void printInfo();
private:
    std::list<NodeMatrixAnimation>channel;
    Setup data;

    friend class ManagerAnimation;
};

/// <summary>
/// AnimationPointer
/// </summary>
class AnimationPointer {
   
public:

    AnimationPointer() : cur_animation(0) {}

    void set(std::vector<Animation>& animations, const std::string& name_node, size_t* _cur_animation);

    inline bool isNull() {
        return channels[*cur_animation] == 0;
    }

    inline NodeMatrixAnimation& get() {
        return *channels[*cur_animation];
    }

private:

    std::vector<NodeMatrixAnimation*>channels;
    size_t* cur_animation;
};

/// <summary>
/// ManagerAnimation
/// </summary>
class ManagerAnimation {

public:
    enum Switch
    {
        timePos_animation_restart, timePos_animation_save
    };
    struct Setup {
        Switch mode_switch = timePos_animation_restart;
        size_t cur_animation;
    };
    void setModeSwitch(Switch state) {
        data.mode_switch = state;
    }
    void create(const aiScene* scene);
    void printInfo();
    inline void switchAnimation(const std::string& name) {
        switchAnimation(name_to_index[name]);
    }
    void switchAnimation(size_t _cur_animation);
    void tick(float time) {
        animation[data.cur_animation].tick(time);
    }
    void getPointer(const std::string& name, AnimationPointer& pointer) {
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

private:
    std::vector<Animation> animation;
    std::map<std::string, size_t>name_to_index;

    Setup data;
};
/// <summary>
/// bVertexMesh
/// </summary>
/// 
class bVertexMesh :public MaterialMesh {

protected:
    // Данные для рендеринга 
    DrawBuffer VAO;
    std::vector<bVertex> vertices;
    std::vector<unsigned int> indices;
    Skeleton* bones;
    // Инициализируем все буферные объекты/массивы
    void setupVO();
    bVertexMesh() {}
    bVertexMesh(aiMesh* mesh, const aiScene* scene, Skeleton* bone_data, const std::string& directory, bool gammaCorreciton);

private:
    // Данные меша
    VertexBufferObject VBO;
    ElementBufferObject EBO;
    // Цикл по всем вершинам меша
    void setVertex(aiMesh* mesh);
    // Проходимся по каждой грани меша (грань - это треугольник меша) и извлекаем соответствующие индексы вершин
    void setIndices(aiMesh* mesh);
    //Кости
    void setBones(aiMesh* mesh);
    void setSingleBone(aiBone* bone);
};

/// <summary>
/// bMesh
/// </summary>
class bMesh :public bVertexMesh, public Drawable,public Transformable3D {

public:
    // Конструктор
    bMesh(aiMesh* mesh, const aiScene* scene, Skeleton* bone_data, const std::string& directory, bool gammaCorreciton);
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

    void draw(const Shader& shader);

    void draw(const View* view, const Shader& shader);

private:

    std::string name;

    void init_data_draw();
};

/// <summary>
/// NodeAnim
/// </summary>
class NodeAnim {

public:
    glm::mat4 transform;
    glm::mat4 global;

    NodeAnim() :default_transform(1.f), transform(1.f), global(1.f) {}
    NodeAnim(const aiNode* node, const glm::mat4& parent_transform, Skeleton& bones, ManagerAnimation& _animation);
    const std::string& getName() {
        return name;
    }
    void bind(Skeleton& bones) {
        bone = bones[name];
    }
    void bind(ManagerAnimation& _animation) {
        _animation.getPointer(name, animation);
    }
    void tick(const glm::mat4& parent_transform, const glm::mat4& inverse_global);

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
        return child[child.size() - 1];
    }

private:
    glm::mat4 default_transform;
    std::vector<NodeAnim> child;
    BonePointer bone;
    AnimationPointer animation;
    std::string name;
};
#endif