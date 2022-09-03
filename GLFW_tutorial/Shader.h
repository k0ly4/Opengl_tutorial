#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Resource_manager.h"
#include"Buffer.h"
struct GeneralShader {
    unsigned int ID;
};
class Shader :public GeneralShader
{
    static unsigned int last_use_shader;
public:
    // Конструктор генерирует шейдер на лету
    Shader() {}
    Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        load(vertexPath, fragmentPath);
    }
    Shader(const std::string& vertexPath, const std::string& geoPath, const std::string& fragmentPath) {
        load(vertexPath, geoPath, fragmentPath);
    }
    void load(const std::string& vertexPath, const std::string& fragmentPath)
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
    void loadDirectory(const std::string& directory,bool geometry_shader =0) {
        if (geometry_shader)load(directory + "shader.vert", directory + "shader.geo", directory + "shader.frag");
        else load(directory + "shader.vert", directory + "shader.frag");
    }
    void load(const std::string& vertexPath, const std::string& geoPath, const std::string& fragmentPath)
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
    inline void use()const
    {
        if (last_use_shader != ID) {
            glUseProgram(ID);
            last_use_shader = ID;
        }
    }
    // Полезные uniform-функции
    bool have(const std::string& name) {
       return glGetUniformLocation(ID, name.c_str())!=-1;
    }
    void uniform(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void uniform(const std::string& name, unsigned int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void uniform(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void uniform(const std::string& name, const std::vector<glm::vec3>& data) const
    {
        for (size_t i = 0; i < data.size(); i++)
            uniform(name + '[' + std::to_string(i) + ']', data[i]);
    }
    void uniform(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void uniform(const std::string& name, const std::vector<glm::mat4>& matrix) const
    {
        for (size_t i = 0; i < matrix.size(); i++)
            uniform(name + '[' + std::to_string(i) + ']', matrix[i]);

    }
    void uniform(const std::string& name, const glm::vec4& vec) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }
    void uniform(const std::string& name, const Color& color) const
    {
        uniform(name, color.vec4());
    }
    void uniform(const std::string& name, const glm::vec3& vec) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }
    void uniform(const std::string& name, const glm::vec2& vec) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    template<typename vector_type_data>
    void uniform(const std::string& name, const std::vector<vector_type_data>& data) {
        for (size_t i = 0; i < data.size(); i++) 
            uniform(name+'['+std::to_string(i) + ']', data[i]);
        
    }
private:
    // Полезные функции для проверки ошибок компиляции/связывания шейдеров
    void checkCompileErrors(unsigned int shader, const std::string type, const  std::string& path)
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
}; 
unsigned int Shader::last_use_shader = -1;
class glShader {
   
    glShader() {}
    ~glShader() {}
    static std::vector<Shader> shader;
    static void setGBuffer(Shader& shader) {
        shader.use();
        shader.uniform("gPosition", 0);
        shader.uniform("gNormal", 1);
        shader.uniform("gAlbedoSpec", 2);
        //shadow-maps
        shader.uniform("d_light.shadow_map[0]", 3);
        /*shader.uniform("d_light.shadow_map[1]", 4);
        shader.uniform("d_light.shadow_map[2]", 5);
        shader.uniform("d_light.shadow_map[3]", 6);*/
    }
    template<typename type_value>
    struct Uniform {
        std::string name;
        type_value value;
        void use(const Shader& shader) const{
            shader.uniform(name, value);
        }
        Uniform(const std::string& _name, type_value _value) :name(_name), value(_value) {}
    };
    template<typename type_value>
    static void setup(size_t index,const std::string& directory,bool geoShader,const Uniform<type_value>& uniform) {
        shader[index].loadDirectory(directory, geoShader);
        if (uniform.name.empty() == 0)
        {  
            shader[index].use();
            uniform.use(shader[index]);
        }
    }
   
public:
    enum Object :size_t
    {
        sprite, font,text, shape, skybox,billboard,
        m_uniform_color, m_layout_color, m_light_layout_color, m_light_uniform_color, m_light_uniform_color_instance,
        m_texture, m_texture_instance, m_texture_normal, m_texture_normal_instance, m_light_texture, 
        gb_texturable, gb_texture_animation,gb_color_uniform, gb_color_uniform_animation, gb_light,
        frame_exposure,
        shadow_depth,cascades_shadow_depth,red,red_array,
        shader_size
    };
    static inline const Shader& get(Object index) {
        return shader[index];
    }
    static void init() {
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

        shader[m_texture].load("shaders\\3d\\texture\\standart\\shader.vert","shaders\\3d\\texture\\standart\\shader.frag");
        shader[m_texture].use();
        shader[m_texture].uniform("diffuse", 0);

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


        shader[gb_light].loadDirectory("shaders\\framebuffer\\gbuffer\\light\\cascade_shadow\\");
        setGBuffer(shader[gb_light]);


        shader[shadow_depth].loadDirectory("shaders\\framebuffer\\gbuffer\\shadow\\depth\\");

        shader[cascades_shadow_depth].loadDirectory("shaders\\framebuffer\\gbuffer\\shadow\\depth\\cascades\\",1);
    }
    static void free() {
        for (auto i : shader) 
            glDeleteShader(i.ID);
    }
};
std::vector<Shader> glShader::shader;


#endif