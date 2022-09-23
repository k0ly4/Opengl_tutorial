#ifndef TERRAIN_H
#define TERRAIN_H
#include "Framebuffer.h"
#include "Camera.h"
#include"Model.h"
class Terrain :public Drawable, public Transformable3D {
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
   
    void GenerateVertex() {
        for (size_t i = 0; i < width_t; i++) {
            for (size_t j = 0; j < height_t; j++) {
                uv[i][j].x = width_t - i - 1;
                uv[i][j].y = j;

                vertices[i][j].x = i;
                vertices[i][j].z = j;
                vertices[i][j].y = (rand() % 10) * 0.02f;
            }
        }
    }
    void GenerateIndex() {
        for (size_t i = 0; i < width_t - 1; i++) {
            size_t  pos = i * height_t;
            for (size_t j = 0; j < height_t - 1; j++) {
                indMap[i][j][0] = pos;
                indMap[i][j][1] = pos + 1;
                indMap[i][j][2] = pos + height_t + 1;

                indMap[i][j][3] = pos + height_t + 1;
                indMap[i][j][4] = pos + height_t;
                indMap[i][j][5] = pos;

                pos++;
            }
        }
    }
    void GenerateHill() {
        for (size_t i = 0; i < 15; i++) {

            createHill(rand() % width_t, rand() % height_t, std::max(rand() % 60, 10), rand() % 10);
        }
    }
    void GeneraNormal() {
        for (size_t i = 0; i < width_t - 1; i++) {
            for (size_t j = 0; j < height_t - 1; j++) {
                normal[i][j] = getNormal(glm::mat3(
                    vertices[i][j],
                    vertices[i + 1][j],
                    vertices[i][j + 1])
                );
            }
        }
    }
    void initVO() {
        VAO.begin();

        VBO_vert.begin();
        VBO_vert.setMode(GL_DYNAMIC_DRAW);
        VBO_vert.data(vertices);
        VAO.attrib(0, 3, 3 * sizeof(float), 0);

        VBO_norm.begin();
        VBO_norm.setMode(GL_DYNAMIC_DRAW);  
        VBO_norm.data(normal);
        VAO.attrib(1, 3, 3 * sizeof(float), 0);
       
        VBO_color.begin();  
        VBO_color.data(uv);
        VAO.attrib(2, 2, 2 * sizeof(float), 0);
       
        EBO.begin();
        EBO.data(indMap);

        VAO.end();
    }
    void init() {
        GenerateVertex();
        GenerateIndex();
        GenerateHill();
        GeneraNormal();

        initVO();
    }


    void GenerateVertex(GlImage& height) {
        std::cout << "NrChannels" << height.nrChannels << std::endl;
        float factor = 0.1f;
        size_t iRow = height.width*height.nrChannels;
        glm::vec2 con = glm::vec2(height.width / width_t, height.height / height_t);
        for (size_t j = 0; j < height_t; j++) {
            float row  = iRow * j * con.y;
        for (size_t i = 0; i < width_t; i++) {
                uv[i][j].x = width_t - i - 1;
                uv[i][j].y = j;

                vertices[i][j].x = i;
                vertices[i][j].z = j;
                size_t index = i * con.x*height.nrChannels + row;
                vertices[i][j].y = height.pixels[index]* factor;
        }
        }
    }
    void init(GlImage& heightMap) {
        GenerateVertex(heightMap);
        GenerateIndex();
        GeneraNormal();
        initVO();
    }

    void createHill(int posX, int posY, int rad, int height) {
        for (int i = posX - rad; i <= posX + rad; i++) {
            for (int j = posY - rad; j <= posY + rad; j++) {

                if (isCoord(i, j)) {
                    float len = sqrt(pow(i - posX, 2) + pow(j - posY, 2));
                    if (len < rad) 
                        vertices[i][j].y += cos(len / rad * PI / 1.9f) * height;
                    
                }
            }
        }
    }
    inline bool isCoord(int x, int z) {
        return (x < vertices.size()-1) && (x >= 0) && (z < vertices[0].size()-1) && (z >= 0);
    }
    inline void toLocal(float& x, float& z) {
        x = (x - getTransform().position.x) / getTransform().scale.x;
        z = (z - getTransform().position.z) / getTransform().scale.z;
    }
    inline void toGlobal(float& y) {
        y = y * getTransform().scale.y + getTransform().position.y;
    }

    friend class Brush;
    inline bool brush_upNormal(int x, int z){
        if (isCoord(x, z)) {
            normal[x][z] = getNormal(glm::mat3(
                vertices[x][z],
                vertices[x + 1][z],
                vertices[x][z + 1])
            );
            return 1;
        }
        return 0;
    }
    inline void brush_moveVertex(int x, int z, float yFactor) {
        if (isCoord(x, z)) {
            vertices[x][z].y += yFactor;
        }
    }
    inline bool brush_getGlobalY(int x,int z,float* y) { 
     if (isCoord(x, z)) {
         *y = vertices[x][z].y;
          toGlobal(*y);
         return 1;
        }
     return 0;
    }
    inline void brush_upVBO() {
       
        VBO_vert.begin();
        VBO_vert.setMode(GL_DYNAMIC_DRAW);
        VBO_vert.data(vertices);
       
       
        VBO_norm.begin();
        VBO_vert.setMode(GL_DYNAMIC_DRAW);
        VBO_norm.data( normal);
       
    }

    inline void resize(const glm::uvec2& size) {
        width_t = size.x;
        height_t = size.y;
      
        mapIntCnt = (size.x-1)  * (size.y-1)  * 6;
        vertices = MatVec3(size.x, std::vector<glm::vec3>(size.y));
        normal = MatVec3(size.x, std::vector<glm::vec3>(size.y));
        uv = MatVec2(size.x, std::vector<glm::vec2>(size.y));
      
        indMap = CubeGluint(size.x - 1,MatGLuint(size.y - 1, std::vector<GLuint>(6)));
    }
public:
    Terrain() {
        id_obj = glShader::gb_texturable;
    }
    void generate(const glm::uvec2& size) {
        resize(size); 
        init();
    }
    void generate(const glm::uvec2& size,const std::string& path_to_height_map) {
        resize(size);
        GlImage image;
        FileManager::loadImage(&image, path_to_height_map);
        init(image);
    }
    bool getHeight(float x, float z, float& height) {
        toLocal(x, z);
        int iX = x;
        int iZ = z;
        if (isCoord(iX, iZ)) {
            float deltaX = x - iX;
            float deltaY = z - iZ;
            float h1 = (1.f - deltaX) * vertices[iX][iZ].y + deltaX * vertices[iX + 1][iZ].y;
            float h2 = (1.f - deltaX) * vertices[iX][iZ + 1].y + deltaX * vertices[iX + 1][iZ + 1].y;
            height = (1.f - deltaY) * h1 + deltaY * h2;
            toGlobal(height);
            return 1;
        }
        return 0;
    }
    void setTexture(Texture2D& texture) {
        this->texture = &texture;
    }
    glm::vec2 getGlobalSize() {
        return glm::vec2(width_t * getTransform().scale.x, height_t * getTransform().scale.y);
    }
    void draw( View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        texture->use(0);
        uniformTransform(shader);
        shader.uniform("specularMaterial", 0.01f);
        VAO.begin();
        glDrawElements(GL_TRIANGLES, mapIntCnt, GL_UNSIGNED_INT, 0);
    }
};

//������� ������������� c ���������������� �������������
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
//������������ ������� �� ������� terrain
class TerrainGrassGenerator {
    static const int grouping = 1000;
    TerrainGrassGenerator() {}
    ~TerrainGrassGenerator() {}
    static void randomize(std::vector<GrassMatrix>& models, const Transform3D& t_local, Terrain& terrain) {
        glm::ivec2 size_terrain = terrain.getGlobalSize();
        size_terrain.x--;
        size_terrain.y--;
        for (size_t i = 0; i < models.size(); i++) {

            glm::vec3 pos(
                terrain.getPosition().x + ((rand() * rand()) % (grouping * size_terrain.x)) / (float)grouping,
                0.f,
                terrain.getPosition().z + ((rand() * rand()) % (grouping * size_terrain.y)) / (float)grouping
            );
            terrain.getHeight(pos.x, pos.z, pos.y);
            models[i].setPosition(t_local.position + glm::vec3(pos.x, pos.y, pos.z));
            models[i].setRotate(Angle3D(rand() % 360, glm::vec3(0.f, 1.f, 0.f)));
            models[i].setScale(t_local.scale);
            models[i].saveModel();
        }
    }
public:
    static void gen(Terrain& terrain, std::vector<GrassMatrix>* models, const Transform3D& t_local, size_t size) {
        models->resize(size);
        randomize(*models, t_local, terrain);
    }
};

//�������� ����� 
class Grass :public Instancable {
    Texture2D* texture;
    DrawBuffer VAO;
    void init() {
        VAO.data_draw = sBuffer::plane.getVAO().data_draw;
        VAO.begin();
        sBuffer::plane.getVBO().begin();
        VAO.attrib(0, 3, 5 * sizeof(float), 0);
        VAO.attrib(1, 2, 5 * sizeof(float), 3 * sizeof(float));
    }
public:
    Grass() {
        id_obj = glShader::m_texture;
        init();
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
    void draw( View* view, const Shader& shader) {
        shader.use();
        view->use(shader);
        uniformTransform(shader);
        uniform(shader);
        VAO.draw();
    }
};

//Instance ������������� ��������� instancable ������� �� ������� terrain
class TerrainGreenery :public Drawable {
    std::vector<GrassMatrix> grass;
    InstanceObject target;
    std::vector<glm::mat4> matrix;
    Terrain* terrain;
    //������������� ������
    void updateMatrix() {
        matrix.resize(grass.size());
        for (size_t i = 0; i < grass.size(); i++)
            matrix[i] = grass[i].model;
    }
public:
    virtual void setObject(Instancable* Object, glShader::Object shader_configuraion) {
        target.setObject(Object, shader_configuraion);
        id_obj = shader_configuraion;
    }
    inline virtual void draw( View* view, const Shader& shader) {
        target.draw(view, shader);
    }
    void create(Terrain& terrain, size_t size) {
        if (target.getObject() == 0) {
            printf("(!)TerrainGreenery::*object==0\n");
            return;
        }
        this->terrain = &terrain;
        TerrainGrassGenerator::gen(terrain, &grass, target.getObject()->getTransform(), size);
        updateMatrix();
        target.create(matrix);
    }
    void upArray(const FloatRect& area) {
        for (size_t i = 0; i < grass.size(); i++) {
            if (area.contain(glm::vec2( grass[i].transform.position.x, grass[i].transform.position.z))) {         
                terrain->getHeight(grass[i].transform.position.x, grass[i].transform.position.z, grass[i].transform.position.y);
                grass[i].transform.position.y += target.getObject()->getTransform().position.y;
                grass[i].saveModel();
                target[i] = grass[i].model;
            }
        }
        target.upVBO();
    }
};

class TerrainGreeneryModel :public Drawable {
    std::vector<GrassMatrix> grass;
    InstanceModel target;
    std::vector<glm::mat4> matrix;
    //������������� ������
    void updateMatrix() {
        matrix.resize(grass.size());
        for (size_t i = 0; i < grass.size(); i++)
            matrix[i] = grass[i].model;
    }
public:
    virtual void setObject(Model& object, glShader::Object shader_configuraion) {
        target.setObject(object, shader_configuraion);
        id_obj = shader_configuraion;
    }
    inline virtual void draw( View* view, const Shader& shader) {
        target.draw(view, shader);
    }
    void create(Terrain& terrain, size_t size) {
        if (target.size() == 0) {
            printf("(!)TerrainGreenery::*object==0\n");
            return;
        }
        TerrainGrassGenerator::gen(terrain, &grass, target.getTransform(), size);
        
        updateMatrix();
        target.create(matrix);
    }
};

class Brush :public Drawable {
    glm::ivec2 pos;
    glm::vec3 mas[4];
    GLuint index[4] = { 0,1,3,2 };
    Terrain* terrain = 0;
    glm::vec3 color;
    DrawBuffer VAO;
    VertexBufferObject VBO;
  
    //  ElementBuffer EBO;
    void computeVertex() {
        size_t k = 0;
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                mas[k].x = pos.x + i;
                mas[k].z = pos.y + j;
                if (!terrain->brush_getGlobalY(mas[k].x, mas[k].z, &mas[k].y))
                    mas[k].y = 0;
                k++;
            }
        }
     
    }
    void upVO() {
        computeVertex();
        VBO.begin();
        VAO.begin();
        VBO.setMode(GL_DYNAMIC_DRAW);
        VBO.data(sizeof(mas), mas);
        VAO.attrib(0, 3, 3 * sizeof(float), 0);

        VAO.end();
        VBO.end();

    }
    bool needUpdateVBO = 0;
public:
    enum MapOper
    {
        UP, DOWN
    }mode = UP;
    Brush() :color(1.f), pos(0.f) {
        id_obj = glShader::m_uniform_color;
        VAO.data_draw = DataDraw(DataDraw::DrawElements, GL_LINE_LOOP, 4);

    }
    MapOper getMode() {
        return mode;
    }
    void setMode(MapOper mode) {
        this->mode = mode;
    }
    void setColor(const glm::vec3&color) {
        this->color = color;
    }
    void upPosition(Camera&camera) {
        float pixel;
        glm::vec2 mouse_pos = glm::vec2(Mouse::getPosition().x, GlRender::Viewport::get().h - Mouse::getPosition().y);
        GlRender::GetPixels(IntRect(mouse_pos, glm::ivec2(1)), GL_DEPTH_COMPONENT, &pixel);
        if (pixel < 1.f) {
            glm::vec3 global = glm::vec3(mouse_pos.x / GlRender::Viewport::get().w, mouse_pos.y / GlRender::Viewport::get().h, pixel) * 2.f - 1.f;
            camera.toGlobal(global);
            // std::cout << "Mouse::x==" << global.x << "y==" << global.y << "z==" << global.z << '\n';
            setPosition(glm::ivec2(global.x, global.z));
    }
    }
    void setPosition(const glm::ivec2& new_pos) {
        pos = new_pos;
        needUpdateVBO = 1;
    }
    FloatRect paint(float strenght) {
        size_t k = 0;
        
        for (size_t i = pos.x; i < pos.x+2; i++) {
            for (size_t j = pos.y; j < pos.y+2; j++) {
             
                if (mode == UP) terrain->brush_moveVertex(i, j, strenght);
                else terrain->brush_moveVertex(i, j, -strenght);
            }
        }
       
        for (size_t i = pos.x -1; i < pos.x+2; i++) {
            for (size_t j = pos.y -1; j < pos.y+2; j++) {
                terrain->brush_upNormal(i,j);
            }
        }
        terrain->brush_upVBO();
        return FloatRect(pos, glm::vec2(3));
    }
    void setTerrain(Terrain* terrain) {
        this->terrain = terrain;
        needUpdateVBO = 1;
    }
    virtual void draw( View* view, const Shader& shader) {
        if (needUpdateVBO) {
            upVO();
            needUpdateVBO = 0;
        }
        GlRender::DepthTest(false);
        shader.use();
        view->use(shader);
        shader.uniform("model", glm::mat4(1.f));
        shader.uniform("color", color);
        GlRender::Line::Width(2.f);
         VAO.begin();
       
       glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, index);
        GlRender::DepthTest(true);
    }
};
#endif