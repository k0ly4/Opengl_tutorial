#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <string>

#include "Resource_manager.h"
#include"Buffer.h"
struct GeneralShader {
    unsigned int ID;
};
class Shader:public GeneralShader
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
    void load(const std::string& vertexPath, const std::string& fragmentPath);
    void loadDirectory(const std::string& directory, bool geometry_shader = 0);
    void load(const std::string& vertexPath, const std::string& geoPath, const std::string& fragmentPath);
    // Активация шейдера
    void use()const;
    // Полезные uniform-функции
    bool have(const std::string& name);
    void uniform(const std::string& name, int value) const;
    void uniform(const std::string& name, unsigned int value) const;
    void uniform(const std::string& name, float value) const;
    void uniform(const std::string& name, const std::vector<glm::vec3>& data) const;
    void uniform(const std::string& name, const glm::mat4& mat) const;
    void uniform(const std::string& name, const std::vector<glm::mat4>& matrix) const;
    void uniform(const std::string& name, const glm::vec4& vec) const;
    void uniform(const std::string& name, const Color& color) const;
    void uniform(const std::string& name, const glm::vec3& vec) const;
    void uniform(const std::string& name, const glm::vec2& vec) const;

    template<typename vector_type_data>
    void uniform(const std::string& name, const std::vector<vector_type_data>& data);
private:
    // Полезные функции для проверки ошибок компиляции/связывания шейдеров
    void checkCompileErrors(unsigned int shader, const std::string type, const  std::string& path);
}; 

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
    static void init();
    static void free() {
        for (auto i : shader) 
            glDeleteShader(i.ID);
    }
};



#endif