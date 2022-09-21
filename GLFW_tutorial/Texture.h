#ifndef TEXTURE_H
#define TEXTURE_H

#include "Shader.h"
#include "Resource_manager.h"
#include <list>


class TextureCubeMap :public GeneralTexture {    

public:
    TextureCubeMap();
    void wrap(unsigned S, unsigned T, unsigned R);
    void wrap(unsigned STR) {
        wrap(STR, STR, STR);
    }
    void filter(unsigned MAG, unsigned MIN);
    void filter(unsigned MAG_MIN) {
        filter(MAG_MIN, MAG_MIN);
    }

    bool load(const std::string& directory, bool flip_vertically = 1, bool gamma = 0);
    inline void use(unsigned int text_unit) const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bindCubeMap(getID());
    }

private:
    // Конфигурация текстуры
    unsigned int Wrap_S; // режим наложения по оси S
    unsigned int Wrap_T; // режим наложения по оси T
    unsigned int Wrap_R; // режим наложения по оси R

    unsigned int Filter_Min; // режим фильтрации, если пикселей текстуры < пикселей экрана
    unsigned int Filter_Max; // режим фильтрации, если пикселей текстуры > пикселей экрана

    void setParameteri();
};
class g_Texture2D :public GeneralTexture {
public:
    inline void use(size_t text_unit)const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bind2D(getID());
    }
    g_Texture2D(const GeneralTexture& Texture) {
        texture = Texture.texture;
    }
    g_Texture2D() {}
};
class Texture2D:public g_Texture2D {

public:
    Texture2D() {}
    Texture2D(const GeneralTexture& Texture, const glm::ivec2& Size) :
        size(Size)
    { texture = Texture.texture; }
    Texture2D(const GeneralTexture& Texture, int width, int height):
        size(glm::uvec2(width,height)) 
    { texture = Texture.texture; }

    bool loadFromFile(const std::string& path_to_image, bool flip_vertically, bool generateMipmap = true, bool gamma = false);
    void loadFromMemory(glm::uvec2 size, GLint internal_format, GLint format, const void* data, bool generateMipmap = true);
    void create(size_t width, size_t height, GLint internal_format, GLint format);
    const glm::ivec2& getSize() {
        return size;
    }

    void wrap(GLint wrap_S, GLint wrap_T);
    inline void wrap(GLint t_wrap) {
        wrap(t_wrap, t_wrap);
    }
    void filter(GLint filter_min, GLint filter_max);
    void filter(GLint t_filter) {
        filter(t_filter, t_filter);
    }
private:
    void setParameteri();
    void init() {
        gen();
        setParameteri();
    }

    // Конфигурация текстуры
    unsigned int Wrap_S; // режим наложения по оси S
    unsigned int Wrap_T; // режим наложения по оси T

    unsigned int Filter_Min; // режим фильтрации, если пикселей текстуры < пикселей экрана
    unsigned int Filter_Max; // режим фильтрации, если пикселей текстуры > пикселей экрана
    glm::ivec2 size;
};
class g_ArrayTexture2D :public GeneralTexture {
public:
    inline void use(size_t text_unit)const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bind2DArray(getID());
    }
    g_ArrayTexture2D(const GeneralTexture& Texture) {
        texture = Texture.texture;
    }
    g_ArrayTexture2D() {}
private:
};
class TextureCubeDepth {
public:
    size_t height = 1024, width = 1024;
    TextureCubeDepth(size_t width, size_t height);
    void bind(unsigned int fbo);
    void use(size_t index) {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_CUBE_MAP, map);
    }
private: 
    unsigned int map;
};
class Image:public GLFWimage {
public:
    Image() {}
    Image(const std::string& path)
    {
        load(path);
    }
    void load(const std::string& path) {
        FileManager::loadImage(this, path,4);
    }
    ~Image() {
        FileManager::freeDataImage(pixels);
    }
private:
};

#endif