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
    uniform(name, color.vec4());
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
void glShader::init() {
    shader.resize(shader_size);
    //2d
    shader[font].load("shaders\\2d\\text\\font\\shader.vert", "shaders\\2d\\text\\font\\shader.frag");
    shader[font].use();
    shader[font].uniform("glyph", 0);

    shader[text].load("shaders\\2d\\text\\text\\shader.vert", "shaders\\2d\\text\\text\\shader.frag");
    shader[text].use();
    shader[text].uniform("glyph", 0);

    shader[sprite].load("shaders\\2d\\sprite\\shader.vert", "shaders\\2d\\sprite\\shader.frag");
    shader[sprite].use();
    shader[sprite].uniform("image", 0);

    shader[shape].load("shaders\\2d\\shape\\shader.vert", "shaders\\2d\\shape\\shader.frag");
    //3d
    shader[skybox].load("shaders\\3d\\skybox\\shader.vert", "shaders\\3d\\skybox\\shader.frag");
    shader[skybox].use();
    shader[skybox].uniform("skybox", 0);

    shader[billboard].loadDirectory("shaders\\3d\\billboard\\perspectable\\", 1);
    shader[billboard].use();
    shader[billboard].uniform("diffuse", 0);

    shader[m_uniform_color].loadDirectory("shaders\\3d\\color\\uniform\\");

    setup(m_layout_color, "shaders\\3d\\color\\layout\\", 0, Uniform<int>("", 0));

    shader[m_texture].load("shaders\\3d\\texture\\standart\\shader.vert", "shaders\\3d\\texture\\standart\\shader.frag");
    shader[m_texture].use();
    shader[m_texture].uniform("diffuse", 0);

    setup(main_texture, "shaders\\3d\\texture\\main\\", 0, Uniform<int>("diffuse", 0));

    shader[m_texture_instance].load("shaders\\3d\\texture\\instance\\shader.vert", "shaders\\3d\\texture\\instance\\shader.frag");
    shader[m_texture_instance].use();
    shader[m_texture_instance].uniform("diffuse", 0);

    shader[m_texture_normal].load("shaders\\3d\\texture\\normal\\shader.vert", "shaders\\3d\\texture\\normal\\shader.frag");
    shader[m_texture_normal].use();
    shader[m_texture_normal].uniform("diffuse1", 0);

    shader[m_texture_normal_instance].load("shaders\\3d\\texture\\normal\\instance\\shader.vert", "shaders\\3d\\texture\\normal\\instance\\shader.frag");
    shader[m_texture_normal_instance].use();
    shader[m_texture_normal_instance].uniform("diffuse1", 0);
    //light
    shader[m_light_layout_color].load("shaders\\3d\\light\\color\\layout\\shader.vert", "shaders\\3d\\light\\color\\layout\\shader.frag");

    shader[m_light_uniform_color].load("shaders\\3d\\light\\color\\uniform\\shader.vert", "shaders\\3d\\light\\color\\uniform\\shader.frag");

    shader[m_light_uniform_color_instance].load("shaders\\3d\\light\\color\\uniform\\instance\\shader.vert", "shaders\\3d\\light\\color\\uniform\\instance\\shader.frag");

    shader[m_light_texture].load("shaders\\3d\\light\\texture\\shader.vert", "shaders\\3d\\light\\texture\\shader.frag");
    shader[m_light_texture].use();
    shader[m_light_texture].uniform("diffuse1", 0);

    shader[gb_texturable].loadDirectory("shaders\\framebuffer\\gbuffer\\object\\texturable\\");
    shader[gb_texturable].use();
    shader[gb_texturable].uniform("diffuse1", 0);

    shader[gb_texture_animation].loadDirectory("shaders\\framebuffer\\gbuffer\\object\\texturable\\animatable\\");
    shader[gb_texture_animation].use();
    shader[gb_texture_animation].uniform("diffuse1", 0);

    shader[gb_color_uniform].loadDirectory("shaders\\framebuffer\\gbuffer\\object\\colorable\\uniform\\");

    shader[gb_color_uniform_animation].loadDirectory("shaders\\framebuffer\\gbuffer\\object\\colorable\\uniform\\animatable\\");

    //frame
    setup(frame_exposure, "shaders\\framebuffer\\exposure\\", 0, Uniform<int>("image", 0));
    setup(red, "shaders\\framebuffer\\gbuffer\\shadow\\red\\", 0, Uniform<int>("image", 0));
    setup(red_array, "shaders\\framebuffer\\gbuffer\\shadow\\red\\array_texture\\", 0, Uniform<int>("image", 0));
    setup(red_font, "shaders\\2d\\text\\red\\", 0, Uniform<int>("image", 0));


    shader[gb_light].loadDirectory("shaders\\framebuffer\\gbuffer\\light\\cascade_shadow\\");
    setGBuffer(shader[gb_light]);

    shader[shadow_depth].loadDirectory("shaders\\framebuffer\\gbuffer\\shadow\\depth\\");
    shader[cascades_shadow_depth].loadDirectory("shaders\\framebuffer\\gbuffer\\shadow\\depth\\cascades\\", 1);
    shader[cube_depth].loadDirectory("shaders\\framebuffer\\gbuffer\\shadow\\depth\\cube\\", 1);
}
    std::vector<Shader> glShader::shader;