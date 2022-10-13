#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <string>

#include "Resource_manager.h"
#include"Buffer.h"

class Shader;


/// GeneralShader
struct GeneralShader {
    unsigned int ID;
};
//////Uniformable
class Uniformable {
    virtual void uniform(const Shader& shader)const = 0;
    friend class Shader;
};

/// Shader-----------------------------------------------
/// <summary>
/// Shader
/// </summary>
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
   
    void uniform(const std::string& name, const glm::mat4& mat) const;
    void uniform(const std::string& name, const std::vector<glm::mat4>& matrix) const;
    void uniform(const std::string& name, const std::vector<glm::vec3>& data) const;

    void uniform(const std::string& name, const Color& color) const;
    void uniform(const std::string& name, const glm::vec4& vec) const;
    void uniform(const std::string& name, const glm::vec3& vec) const;
    void uniform(const std::string& name, const glm::vec2& vec) const;

    template<typename type_array>
    void uniform(const std::string& name, size_t size_element,const type_array* data)const {
        for (size_t i = 0; i < size_element; i++)
            uniform(name + '[' + std::to_string(i) + ']', data[i]);
    }
    template<typename vector_type_data>
    void uniform(const std::string& name, const std::vector<vector_type_data>& data) {
        for (size_t i = 0; i < data.size(); i++)
            uniform(name + '[' + std::to_string(i) + ']', data[i]);
    }
    void uniform(const Uniformable& object)const {
        object.uniform(*this);
    }
  

private:

    // Полезные функции для проверки ошибок компиляции/связывания шейдеров
    void checkCompileErrors(unsigned int shader, const std::string type, const  std::string& path);
}; 

/// <summary>
/// Uniform
/// </summary>
template<typename type_value>
struct Uniform {
    std::string name;
    type_value value;
    void use(const Shader& shader) const {
        shader.uniform(name, value);
    }
    Uniform(const std::string& _name, type_value _value) :name(_name), value(_value) {}
};

/// glShader-----------------------------------------------
/// <summary>
/// glShader
/// </summary>
class glShader {
  
public:

    enum Object :size_t
    {
        font,
        text,
        shape,
        skybox,
        billboard,

        color_layout,

        texture,
        texture_loc2,
        texture_instance,
        texture_instance_loc2,

        //forward render
        texture_pointLight,
        texture_dirLight,
        //gBuffer
        gb_texture,
        gb_texture_animation,
        gb_render_dir_lights,
        gb_render_dir_lights_test,
        gb_render_point_lights,
        //posEffects
        frame_exposure,
        //Render in shadow map
        depth,
        depth_cascaded,
        depth_cube,
        //Debug frame in one channel
        red,
        red_array,
        //Total size
        shader_size,
        any,
        any_skeletal_animation,
        any_light_texture,
    };

    static inline const Shader& get(Object index) {
        return shader[index];
    }

    static void init();

    static void free() {
        for (auto& i : shader) 
            glDeleteShader(i.ID);
    }

private:

    glShader() {}
    ~glShader() {}

    static std::vector<Shader> shader;

    

    template<typename type_value>
    static void setup(size_t index, const std::string& directory, bool geoShader, const Uniform<type_value>& uniform) {
        shader[index].loadDirectory(directory, geoShader);
        if (uniform.name.empty() == 0)
        {
            shader[index].use();
            uniform.use(shader[index]);
        }
    }
};



#endif