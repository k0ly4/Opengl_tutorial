#include "Mesh.h"

/// <summary>
/// Asset
/// </summary>
/// 
bool Asset::load(const std::string& path, bool gamma) {
    const TextureResource* resource = ImageLoader::getTexture(path, gamma);
    if (resource) {
        texture = Texture2D(*resource);
        return 1;
    }
    return 0;
}

/// <summary>
/// MaterialMesh
/// </summary>
/// 
void MaterialMesh::addAssets(aiMaterial* mat, const std::string& directory, aiTextureType type, Asset::TYPE typeName, bool gammaCorrection)
{
    aiString str;

    for (size_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        mat->GetTexture(type, i, &str);
        log("MaterialMesh::addAssets "+std::string(str.C_Str())+"\n");
        texture.push_back(Asset(directory + '/' + std::string(str.C_Str()), typeName, gammaCorrection));
    }
}

void MaterialMesh::addMaterial(aiMaterial* material, const std::string& directory, bool gammaCorrection) {
    // Мы вводим соглашение об именах сэмплеров в шейдерах. Каждая диффузная текстура будет называться 'texture_diffuseN',
    // где N - это порядковый номер от 1 до MAX_SAMPLER_NUMBER. 
    // То же самое относится и к другим текстурам:
    // диффузная: texture_diffuseN
    // отражения: texture_specularN
    // нормали: texture_normalN
    // 1. Диффузные карты
    addAssets(material, directory, aiTextureType_DIFFUSE, Asset::diffuse, gammaCorrection);
    // 2. Карты отражения
    addAssets(material, directory, aiTextureType_SPECULAR, Asset::specular, 0);
    // 3. Карты нормалей
    addAssets(material, directory, aiTextureType_HEIGHT, Asset::normal, 0);
    // 4. Карты высот
    addAssets(material, directory, aiTextureType_AMBIENT, Asset::height, 0);
}

void MaterialMesh::bindAssets(const Shader& shader) {
    unsigned int num_text[Asset::TYPE::size] = { 1,1,1,1 };
    for (size_t i = 0; i < texture.size(); i++)
    {
        // Получаем texture текстуры (номер N в diffuse_textureN)
        num_text[texture[i].type]++;
        // Теперь устанавливаем сэмплер на нужный текстурный юнит
        shader.uniform(name_shader_uniform[texture[i].type] + std::to_string(num_text[texture[i].type]), i);
        // и связываем текстуру
        texture[i].texture.use(GL_TEXTURE0 + i);// перед связыванием активируем нужный текстурный юнит
    }
}

void MaterialMesh::uniformMaterial(const Shader& shader) {
    shader.uniform("specularMaterial", specularFactor);
    if (texturable)bindAssets(shader);
    else shader.uniform("color", color);
}

void MaterialMesh::setMaterials(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gamma) {
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
/// <summary>
/// VertexMesh
/// </summary>
/// 
void VertexMesh::setVertex(aiMesh* mesh) {
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
void VertexMesh::setIndices(aiMesh* mesh) {

    indices.reserve(mesh->mNumFaces * 3);
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        // Получаем все индексы граней и сохраняем их в векторе indices
        for (size_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
}

VertexMesh::VertexMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gammaCorreciton) {
    texturable = mesh->mTextureCoords[0];
    std::cout << "Texturable:" << texturable << std::endl;
    setVertex(mesh);
    setIndices(mesh);
    setupVO();
    setMaterials(mesh, scene, directory, gammaCorreciton);
}

void VertexMesh::setupVO()
{
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
    VAO.attrib(0, 3, SIZE_VERTEX, 0); // Координаты вершин
    VAO.attrib(1, 3, SIZE_VERTEX, offsetof(Vertex, normal)); // Нормали вершин
    VAO.attrib(2, 2, SIZE_VERTEX, offsetof(Vertex, texCoord));  // Текстурные координаты вершин

    VAO.end();
}
/// <summary>
/// Mesh
/// </summary>
/// 
Mesh::Mesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, bool gammaCorreciton) :
    VertexMesh(mesh, scene, directory, gammaCorreciton) {
    name = mesh->mName.C_Str();
    init_data_draw();
}

void Mesh::draw(View* view, const Shader& shader)
{
    shader.use();
    view->use(shader);
    uniform(shader);
    uniformTransform(shader);
    VAO.draw();
}
