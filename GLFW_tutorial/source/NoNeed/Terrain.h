#ifndef TERRAIN_H
#define TERRAIN_H

#include "Framebuffer.h"
#include "Model.h"
#include "PrimitiveEntity.h"
#include "Mouse.h"

/// <summary>
/// Terrain
/// </summary>
class Terrain :public Drawable, public Transformable3D {
    
public:

    Terrain() {
        id_obj = glShader::gb_texturable;
    }
    void generate(const glm::uvec2& size) {
        resize(size); 
        initialize();
    }
    void generate(const glm::uvec2& size,const std::string& path_to_height_map) {
        resize(size);
        GlImage image;
        FileManager::loadImage(&image, path_to_height_map);
        initialize(image);
    }
    bool getHeight(float x, float z, float& height);
    void setTexture(Texture2D& texture) {
        this->texture = &texture;
    }
    glm::vec2 getGlobalSize() {
        return glm::vec2(width_t * getTransform().scale.x, height_t * getTransform().scale.y);
    }
    void draw(View* view, const Shader& shader);

private:

        size_t width_t, height_t;
        size_t mapIntCnt;

        typedef  std::vector<std::vector<glm::vec3>> MatVec3;
        typedef  std::vector<std::vector<glm::vec2>> MatVec2;
        typedef std::vector<std::vector<GLuint>> MatGLuint;
        typedef std::vector<MatGLuint> CubeGluint;

        MatVec3 vertices;
        MatVec2 uv;
        MatVec3 normal;
        CubeGluint indMap;

        DrawBuffer VAO;
        ElementBufferObject EBO;
        VertexBufferObject VBO_vert, VBO_color, VBO_norm;

        Texture2D* texture;

        void generateVertex();
        void generateVertex(GlImage& height);

        void generateIndex();;
        void generateHill();
        void generaNormal();

        void initializeVO();

        void initialize() {
            generateVertex();
            generateIndex();
            generateHill();
            generaNormal();

            initializeVO();
        }
        void initialize(GlImage& heightMap) {
            generateVertex(heightMap);
            generateIndex();
            generaNormal();

            initializeVO();
        }

        void createHill(int posX, int posY, int rad, int height);

        inline bool isCoord(int x, int z) {
            return (x < vertices.size() - 1) && (x >= 0) && (z < vertices[0].size() - 1) && (z >= 0);
        }
        inline void toLocal(float& x, float& z) {
            x = (x - getTransform().position.x) / getTransform().scale.x;
            z = (z - getTransform().position.z) / getTransform().scale.z;
        }
        inline void toGlobal(float& y) {
            y = y * getTransform().scale.y + getTransform().position.y;
        }

        friend class Brush;
        inline bool brush_upNormal(int x, int z);
        inline void brush_moveVertex(int x, int z, float yFactor);
        inline bool brush_getGlobalY(int x, int z, float* y);
        inline void brush_upVBO();

        inline void resize(const glm::uvec2& size);
};

/// <summary>
/// GrassMatrix
/// //Матрицы трансформации c оптимизационными возможностями
/// </summary>
/// 
struct GrassMatrix :public InstanceMatrix {
    float length;
    inline float sumCoord(const glm::vec3& vec) {
        return abs(vec.x) + abs(vec.y) + abs(vec.z);
    }
    inline void setViewPos(const glm::vec3& viewPos) {
        length = sumCoord(transform.position - viewPos);
    }
    static bool comp(const GrassMatrix& left, const GrassMatrix& right) {
        return left.length < right.length;
    }
};

/// <summary>
/// TerrainGrassGenerator
/// Распределяет матрицы по площади terrain
/// </summary>
/// 
class TerrainGrassGenerator {
    
public:

    static void gen(Terrain& terrain, std::vector<GrassMatrix>* models, const Transform3D& t_local, size_t size) {
        models->resize(size);
        randomize(*models, t_local, terrain);
    }

private:

    static const int grouping = 1000;

    TerrainGrassGenerator() {}
    ~TerrainGrassGenerator() {}
    
    static void randomize(std::vector<GrassMatrix>& models, const Transform3D& t_local, Terrain& terrain);
};

/// <summary>
/// Grass
/// //Тестовая трава 
/// </summary>
/// 
class Grass :public Instancable {

public:

    Grass() {
        id_obj = glShader::m_texture;
        initialize();
    }

    void setTexture(Texture2D& texture) {
        this->texture = &texture;
    }

    const DrawBuffer& getDrawBuffer() {
        return VAO;
    }

    inline void uniform(const Shader& shader) {
        texture->use(0);
    }

    void draw(View* view, const Shader& shader);

private:
    Texture2D* texture;
    DrawBuffer VAO;
    void initialize();
};

/// <summary>
/// TerrainGreenery
/// //Instance рапределитель заданного instancable объекта по площади terrain
/// </summary>
/// 
class TerrainGreenery :public Drawable {

public:

    virtual void setObject(Instancable* Object, glShader::Object shader_configuraion) {
        target.setObject(Object, shader_configuraion);
        id_obj = shader_configuraion;
    }

    inline virtual void draw( View* view, const Shader& shader) {
        target.draw(view, shader);
    }

    void create(Terrain& terrain, size_t size);
    void upArray(const FloatRect& area);

private:

    std::vector<GrassMatrix> grass;
    InstanceObject target;
    std::vector<glm::mat4> matrix;
    Terrain* terrain;
    //Интерпритация матриц
    void updateMatrix();
};

/// <summary>
/// TerrainGreeneryModel
/// </summary>
class TerrainGreeneryModel :public Drawable {
  
public:

    virtual void setObject(Model& object, glShader::Object shader_configuraion) {
        target.setObject(object, shader_configuraion);
        id_obj = shader_configuraion;
    }
    inline virtual void draw( View* view, const Shader& shader) {
        target.draw(view, shader);
    }

    void create(Terrain& terrain, size_t size);

private:

    std::vector<GrassMatrix> grass;
    InstanceModel target;
    std::vector<glm::mat4> matrix;
    //Интерпритация матриц
    void updateMatrix();
};

/// <summary>
/// Brush
/// </summary>
class Brush :public Drawable {
   
public:

    enum MapOper
    {
        UP, DOWN
    }mode = UP;

    Brush() :color(1.f), pos(0.f) {
        id_obj = glShader::m_uniform_color;
        VAO.data_draw = DataDraw(DataDraw::DrawElements, GL_LINE_LOOP, 4);

    }
    void setMode(MapOper mode) {
        this->mode = mode;
    }
    MapOper getMode() {
        return mode;
    }
    void setColor(const glm::vec3&color) {
        this->color = color;
    }
    void setPosition(const glm::ivec2& new_pos) {
        pos = new_pos;
        needUpdateVBO = 1;
    }
    void setTerrain(Terrain* terrain) {
        this->terrain = terrain;
        needUpdateVBO = 1;
    }

    void upPosition(Camera& camera);

    FloatRect paint(float strenght);
    
    virtual void draw(View* view, const Shader& shader);

private:

    glm::ivec2 pos;
    glm::vec3 mas[4];
    GLuint index[4] = { 0,1,3,2 };
    Terrain* terrain = 0;
    glm::vec3 color;
    DrawBuffer VAO;
    VertexBufferObject VBO;

    //  ElementBuffer EBO;
    void computeVertex();
    void upVO();
    bool needUpdateVBO = 0;
};
#endif