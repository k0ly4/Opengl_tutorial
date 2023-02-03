#include "Shader.h"

void Shader::load(const std::string& vertexPath, const std::string& fragmentPath)
{
    // Этап №1: Получение исходного кода вершинного/фрагментного шейдера из переменной filePath
    std::string vertexCode;
    std::string fragmentCode;

    FileManager::read(vertexCode, vertexPath);
    FileManager::read(fragmentCode, fragmentPath);
    // Этап №2: Компилируем шейдеры
    unsigned int vertex, fragment;

    // Вершинный шейдер
    vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* code = vertexCode.c_str();
    glShaderSource(vertex, 1, &code, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX", vertexPath);

    // Фрагментный шейдер
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    code = fragmentCode.c_str();
    glShaderSource(fragment, 1, &code, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT", fragmentPath);

    // Шейдерная программа
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM", "");

    // После того, как мы связали шейдеры с нашей программой, удаляем их, так как они нам больше не нужны
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::loadDirectory(const std::string& directory, bool geometry_shader) {
    if (geometry_shader)load(directory + "shader.vert", directory + "shader.geo", directory + "shader.frag");
    else load(directory + "shader.vert", directory + "shader.frag");
}

void Shader::load(const std::string& vertexPath, const std::string& geoPath, const std::string& fragmentPath)
{
    // Этап №1: Получение исходного кода вершинного/фрагментного шейдера из переменной filePath
    std::string vertexCode,
        geoCode,
        fragmentCode;

    FileManager::read(vertexCode, vertexPath);
    FileManager::read(geoCode, geoPath);
    FileManager::read(fragmentCode, fragmentPath);

    // Этап №2: Компилируем шейдеры
    unsigned int vertex, fragment, geometry;

    // Вершинный шейдер
    vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* code = vertexCode.c_str();
    glShaderSource(vertex, 1, &code, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX", vertexPath);
    // Геометрия шейдер
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    code = geoCode.c_str();
    glShaderSource(geometry, 1, &code, NULL);
    glCompileShader(geometry);
    checkCompileErrors(geometry, "GEOMETRY", geoPath);

    // Фрагментный шейдер
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    code = fragmentCode.c_str();
    glShaderSource(fragment, 1, &code, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT", fragmentPath);

    // Шейдерная программа
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, geometry);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM", "");

    // После того, как мы связали шейдеры с нашей программой, удаляем их, так как они нам больше не нужны
    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
}
// Активация шейдера
void Shader::use()const
{
    if (last_use_shader != ID) {
        glUseProgram(ID);
        last_use_shader = ID;
    }
}
// Полезные uniform-функции
bool Shader::have(const std::string& name) {
    return glGetUniformLocation(ID, name.c_str()) != -1;
}
void Shader::uniform(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::uniform(const std::string& name, unsigned int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::uniform(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::uniform(const std::string& name, const std::vector<glm::vec3>& data) const
{
    for (size_t i = 0; i < data.size(); i++)
        uniform(name + '[' + std::to_string(i) + ']', data[i]);
}
void Shader::uniform(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::uniform(const std::string& name, const std::vector<glm::mat4>& matrix) const
{
    for (size_t i = 0; i < matrix.size(); i++)
        uniform(name + '[' + std::to_string(i) + ']', matrix[i]);
}
void Shader::uniform(const std::string& name, const glm::vec4& vec) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}
void Shader::uniform(const std::string& name, const Color& color) const
{
    uniform(name, color.vec4);
}
void Shader::uniform(const std::string& name, const glm::vec3& vec) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}
void Shader::uniform(const std::string& name, const glm::vec2& vec) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

// Полезные функции для проверки ошибок компиляции/связывания шейдеров
void Shader::checkCompileErrors(unsigned int shader, const std::string type, const  std::string& path)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR(" + path + ") of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR(" + path + ") of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
unsigned int Shader::last_use_shader = -1;



/// <summary>
/// glShader
/// </summary>
/// 
/// 
void setGBufferPointLights(Shader& shader) {
    shader.use();
    shader.uniform("gPosition", 0);
    shader.uniform("gNormal", 1);
    shader.uniform("gAlbedoSpec", 2);
    //shadow-maps
    shader.uniform("p_light[0].map", 3);
}

void setGbufferDirLights(Shader& shader) {
    shader.use();
    shader.uniform("gPosition", 0);
    shader.uniform("gNormal", 1);
    shader.uniform("gAlbedoSpec", 2);
    //shadow-maps
    shader.uniform("d_light.map", 3);
}

void setLightForwardPointLight(Shader& shader) {
    shader.use();
    //shadow-maps
    //material neccessery
    shader.uniform("diffuse", 0);
    //light
    shader.uniform("p_light[0].map", 1);
}
void setLightForwardDirLight(Shader& shader) {
    shader.use();
    //shadow-maps
    //material neccessery
    shader.uniform("diffuse", 0);
    //light
    shader.uniform("d_light.map", 1);
}

void glShader::init() {
    shader.resize(shader_size);
    //2d
    shader[shape].loadDirectory("shaders\\2d\\shape\\");
    shader[convex2d].loadDirectory("shaders\\2d\\convex\\");
    setup(texture_2d, "shaders\\2d\\texture\\", 0, Uniform<int>("diffuse", 0));

    setup(text,"shaders\\3d\\text\\", 0, Uniform<int>("glyph", 0));
    //3d
    setup(voxel, "shaders\\main\\voxel\\", 0, Uniform<int>("diffuse", 0));

    setup(skybox, "shaders\\3d\\skybox\\", 0, Uniform<int>("skybox", 0));

    setup(billboard, "shaders\\3d\\billboard\\", 1, Uniform<int>("diffuse", 0));

    setup(color_layout, "shaders\\3d\\non-texture\\layout\\", 0, Uniform<int>("", 0));

    setup(test, "shaders\\main\\test\\", 0, Uniform<int>("diffuse", 0));
    setup(texture, "shaders\\3d\\default\\", 0, Uniform<int>("diffuse", 0));
    setup(texture_loc2, "shaders\\3d\\texture\\location2\\default\\", 0, Uniform<int>("diffuse", 0));

    setup(texture_instance, "shaders\\3d\\texture\\location1\\instance\\", 0, Uniform<int>("diffuse", 0));
    setup(texture_instance_loc2, "shaders\\3d\\texture\\location2\\instance\\", 0, Uniform<int>("diffuse", 0));
    //forward
    shader[texture_pointLight].loadDirectory("shaders\\light\\forward\\default\\pointLight\\");
    setLightForwardPointLight(shader[texture_pointLight]);

    shader[texture_dirLight].loadDirectory("shaders\\light\\forward\\default\\dirLight\\");
    setLightForwardDirLight(shader[texture_dirLight]);
    //gBuffer
    setup(gb_texture, "shaders\\light\\gbuffer\\object\\default\\", 0, Uniform<int>("diffuse", 0));
    setup(gb_texture_animation, "shaders\\light\\gbuffer\\object\\animatable\\", 0, Uniform<int>("diffuse", 0));

    shader[gb_render_point_lights].loadDirectory("shaders\\light\\gBuffer\\render\\pointLight\\");
    setGBufferPointLights(shader[gb_render_point_lights]);

    shader[gb_render_dir_lights].loadDirectory("shaders\\light\\gBuffer\\render\\directionLight\\");
    setGbufferDirLights(shader[gb_render_dir_lights]);

    shader[gb_render_dir_lights_test].loadDirectory("shaders\\light\\gBuffer\\render\\simpleDirLight\\");
    setGbufferDirLights(shader[gb_render_dir_lights_test]);

    //frame
    setup(frame_exposure, "shaders\\framebuffer\\exposure\\", 0, Uniform<int>("image", 0));
    setup(frame, "shaders\\framebuffer\\forward\\", 0, Uniform<int>("image", 0));
    setup(red, "shaders\\light\\shadow\\display\\default\\", 0, Uniform<int>("image", 0));
    setup(red_array, "shaders\\light\\shadow\\display\\array_texture\\", 0, Uniform<int>("image", 0));
    setup(font, "shaders\\framebuffer\\font\\", 0, Uniform<int>("glyph", 0));

    shader[depth].loadDirectory("shaders\\light\\shadow\\render\\default\\");
    shader[depth_cascaded].loadDirectory("shaders\\light\\shadow\\render\\cascaded\\", 1);
    shader[depth_cube].loadDirectory("shaders\\light\\shadow\\render\\cube\\", 1);
}
    std::vector<Shader> glShader::shader;