#ifndef TEXTURE_H
#define TEXTURE_H

#include "Shader.h"
#include "Resource_manager.h"
#include <list>


class TextureCubeMap :public GeneralTexture {    
    // Конфигурация текстуры
    unsigned int Wrap_S; // режим наложения по оси S
    unsigned int Wrap_T; // режим наложения по оси T
    unsigned int Wrap_R; // режим наложения по оси R

    unsigned int Filter_Min; // режим фильтрации, если пикселей текстуры < пикселей экрана
    unsigned int Filter_Max; // режим фильтрации, если пикселей текстуры > пикселей экрана

    void setParameteri() {
        glTexture::bindCubeMap(getID());
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, Wrap_S);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, Wrap_T);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, Wrap_R);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, Filter_Max);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, Filter_Min);
    }
public:
    TextureCubeMap() {
        wrap(GL_CLAMP_TO_EDGE);
        filter(GL_LINEAR);
        gen();
        setParameteri();
    }
    void wrap(unsigned S, unsigned T, unsigned R) {
        Wrap_S = S;
        Wrap_T = T;
        Wrap_R = R;
        if (texture) setParameteri();
    }
    void filter(unsigned MAG, unsigned MIN) {
        Filter_Min = MIN;
        Filter_Max = MAG;
        if (texture) setParameteri();
    }
    void wrap(unsigned STR){
        wrap(STR, STR, STR);
    }
    void filter(unsigned MAG_MIN) {
        filter(MAG_MIN, MAG_MIN);
    }

    bool load(const std::string& directory, bool flip_vertically = 1,bool gamma =0) {
        stbi_set_flip_vertically_on_load(flip_vertically);
        std::string path[6] = {
           "right.jpg","left.jpg","top.jpg","bottom.jpg","front.jpg","back.jpg"
        };
        glTexture::bindCubeMap(getID());
        for (int i = 0; i < 6; i++) {
            GlImage data;
            if (FileManager::loadImage(&data, directory + path[i]))
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, data.internal_format(gamma), data.width, data.height, 0, data.format(), GL_UNSIGNED_BYTE, data.pixels);
            else return 0;
        }
        glTexture::bindCubeMap(0);
        return 1;
    }
    inline void use(unsigned int text_unit) const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bindCubeMap(getID());
    }
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
    void setParameteri() {
        glTexture::bind2D(getID());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
    }
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
public:
    Texture2D() {}
    Texture2D(const GeneralTexture& Texture, const glm::ivec2& Size) :
        size(Size)
    { texture = Texture.texture; }
    Texture2D(const GeneralTexture& Texture, int width, int height):
        size(glm::uvec2(width,height)) 
    { texture = Texture.texture; }

    bool loadFromFile(const std::string& path_to_image,bool flip_vertically, bool generateMipmap =true, bool gamma = false) {
        stbi_set_flip_vertically_on_load(flip_vertically);
        if (FileManager::loadTexture(path_to_image,this,&size,gamma)) {
            setParameteri();
            if(generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);
            return 1;
        }
        return 0;
    }
    void loadFromMemory(glm::uvec2 size, GLint internal_format, GLint format, const void* data, bool generateMipmap = true) {
        init();
        this->size = size;
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0, format, 
           internal_format == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE, data);
        if(generateMipmap)glGenerateMipmap(GL_TEXTURE_2D);
    }
    void create(size_t width,size_t height,GLint internal_format,GLint format) {
        init();       
        size = glm::uvec2(width, height);
        GLenum type;
        if (internal_format == GL_RGBA16F || internal_format == GL_DEPTH_COMPONENT) type = GL_FLOAT;
        else type = GL_UNSIGNED_BYTE;

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, NULL);
    }
    const glm::ivec2& getSize() {
        return size;
    }

    void wrap(GLint wrap_S, GLint wrap_T) {
        Wrap_S = wrap_S;
        Wrap_T = wrap_T;
        if (texture) {
            glTexture::bind2D(getID());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
        }
    }
    inline void wrap(GLint t_wrap) {
        wrap(t_wrap, t_wrap);
    }
    void filter(GLint filter_min, GLint filter_max) {
             Filter_Min = filter_min;
             Filter_Max = filter_max;
    
             if (texture) {
                 glTexture::bind2D(getID());
                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);///GL_LINEAR
                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);//GL_LINEAR_MIPMAP_LINEAR
             }
    }
    void filter(GLint t_filter) {
        filter(t_filter, t_filter);
    }
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

};
class TextureCubeDepth {
    unsigned int map;
public:
    size_t height = 1024, width = 1024;
    TextureCubeDepth(size_t width, size_t height) {
        glGenTextures(1, &map);
        glBindTexture(GL_TEXTURE_CUBE_MAP, map);
        for (size_t i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        /*   float color_border[] = { 1.f,1.f,1.f,1.f };
           glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, color_border);*/
        this->height = height;
        this->width = width;
    }
    void bind(unsigned int fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, map, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void use(size_t index) {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_CUBE_MAP, map);
    }
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
};

#endif