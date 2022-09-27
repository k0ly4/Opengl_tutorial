#ifndef TEXTURE_H
#define TEXTURE_H

#include "Shader.h"
#include "Resource_manager.h"
#include <list>
#include "Exception.h"

/// <summary>
/// TextureFilter
/// </summary>
class TextureFilter {
public:

    enum Mode: unsigned int
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,

    };

    TextureFilter():
        filter_min_(NearestMipmapLinear),
        filter_max_(Linear) 
    {}

    void set(unsigned mag, unsigned min) {
        filter_min_ = min;
        filter_max_ = mag;
    }

    void setup(GLenum target) {

        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter_max_);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter_min_);

    }

    void setup(GLenum target, unsigned mag, unsigned min) {
        set(mag, min);
        setup(target);
    }

protected:

    unsigned filter_min_; // режим фильтрации, если пикселей текстуры < пикселей экрана
    unsigned filter_max_; // режим фильтрации, если пикселей текстуры > пикселей экрана

};


/// <summary>
/// TextureWrap
/// </summary>
class TextureWrap {
public:
    enum Mode : unsigned int
    {
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        Repeat = GL_REPEAT,
    };

    inline void setBorderColor(GLenum target, const glm::vec4& borderColor) {
        glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
    }

    virtual void setup(GLenum target) = 0;
};

/// <summary>
/// TextureWrap3D
/// </summary>
class TextureWrap3D:public TextureWrap {

public:

    TextureWrap3D():
        Wrap_S(Repeat),
        Wrap_T(Repeat),
        Wrap_R(Repeat)
    {}

    void set(unsigned S, unsigned T, unsigned R) {
        Wrap_S = S;
        Wrap_T = T;
        Wrap_R = R;
    }

    void setup(GLenum target) {

        glTexParameteri(target, GL_TEXTURE_WRAP_S, Wrap_S);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, Wrap_T);
        glTexParameteri(target, GL_TEXTURE_WRAP_R, Wrap_R);
        
    }

    void setup(GLenum target, unsigned S, unsigned T, unsigned R) {
        set(S, T, R);
        setup(target);
    }

    glm::uvec3 get()const {
        return glm::uvec3(Wrap_S, Wrap_T, Wrap_R);
    }

protected:
    // Конфигурация текстуры
    unsigned int Wrap_S; // режим наложения по оси S
    unsigned int Wrap_T; // режим наложения по оси T
    unsigned int Wrap_R; // режим наложения по оси R

};

/// <summary>
/// TextureWrap2D
/// </summary>
class TextureWrap2D:public TextureWrap {
public:

    TextureWrap2D() :
        Wrap_S(Repeat),
        Wrap_T(Repeat)
    {}

    void set(unsigned S, unsigned T) {
        Wrap_S = S;
        Wrap_T = T;  
    }

    void setup(GLenum target) {
        glTexParameteri(target, GL_TEXTURE_WRAP_S, Wrap_S);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, Wrap_T);
    }

    void setup(GLenum target, unsigned S, unsigned T) {
        set(S, T);
        setup(target);
    }

    glm::uvec2 get()const {
        return glm::uvec2(Wrap_S, Wrap_T);
    }

protected:
    // Конфигурация текстуры
    unsigned int Wrap_S; // режим наложения по оси S
    unsigned int Wrap_T; // режим наложения по оси T
};



/// <summary>
/// TextureCubeMap
/// </summary>
class TextureCubeMap:public Texture {

public:
    TextureCubeMap();

    bool loadFromDirectory(const std::string& directory, bool flipVertically = 1, bool gammaMod = 0);

    void wrap(unsigned S, unsigned T, unsigned R) {
        glTexture::bindCubeMap(id_);
        wrap_.setup(GL_TEXTURE_CUBE_MAP, S, T, R);
    }

    void wrap(unsigned STR) {
        wrap(STR, STR, STR);
    }

    void filter(unsigned min,unsigned mag){
        glTexture::bindCubeMap(id_);
        filter_.setup(GL_TEXTURE_CUBE_MAP, mag, min);
    }

    inline void use(unsigned int text_unit) const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bindCubeMap(id_);
    }

private:

    TextureFilter filter_;
    TextureWrap3D wrap_;
};

/// <summary>
/// GeneralTexture2D
/// </summary>
class GeneralTexture2D :public Texture {
public:

    inline void use(size_t text_unit)const {

        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bind2D(id_);
    }

    GeneralTexture2D(const TexturePointer& id):
        Texture(id)
    {}

    GeneralTexture2D() {}

protected:
};

/// <summary>
/// Texture2D
/// </summary>
class Texture2D:public GeneralTexture2D{

public:

    Texture2D():
        isGenerateMipmap_(0),
        size_(0)
    {}

    Texture2D(const TexturePointer& textureId, const glm::ivec2& size):
        size_(size),
        GeneralTexture2D(textureId),
        isGenerateMipmap_(0)
    {}

    Texture2D(const TextureResource&resource):
        Texture2D(resource.getId(),resource.getSize())
    {}

    Texture2D(const TexturePointer& textureId, int width, int height) :
        Texture2D(textureId, glm::ivec2(width, height))
    {}

    bool loadFromFile(const std::string& path_to_image, bool generateMipmap = 1, bool gammaMod = 1);
    void loadFromMemory(const glm::ivec2& size, GLint internal_format, GLint format, const void* data, bool generateMipmap = 1);
    void create(int width, int height, GLint internal_format, GLint format);

    const glm::ivec2& getSize()const {
        return size_;
    }

    void wrap(GLint wrap_S, GLint wrap_T) {
        glTexture::bind2D(id_);
        wrap_.setup(GL_TEXTURE_2D, wrap_S, wrap_T);
    }

    inline void wrap(GLint ST) {
        wrap(ST, ST);
    }

    void filter(GLint filter_min, GLint filter_max) {      
        glTexture::bind2D(id_);
        filter_.setup(GL_TEXTURE_2D, filter_max, filter_min);
    }

    inline void filter(GLint filterMinMax) {
        filter(filterMinMax, filterMinMax);
    }

    bool haveMipmaps() {
        return isGenerateMipmap_;
    }
private:

    void detach() {
        id_.reBuild();
        glTexture::bind2D(id_);
        wrap_.setup(GL_TEXTURE_2D);
        filter_.setup(GL_TEXTURE_2D);
    }

    bool isGenerateMipmap_;
    glm::ivec2 size_;
    TextureFilter filter_;
    TextureWrap2D wrap_;
};

/// <summary>
/// GeneralTexture2D
/// </summary>
class ArrayTexture2D:public Texture {
public:

   
    ArrayTexture2D(const TexturePointer& id):Texture(id)
    {}

    ArrayTexture2D() {}

    inline void use(size_t text_unit)const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bind2DArray(id_);
    }

    inline void detach() {
        id_.reBuild();
    }
private:
};

/// <summary>
/// TextureCubeDepth
/// </summary>
class TextureCubeDepth {
public:
    size_t height = 1024, width = 1024;
    TextureCubeDepth(int width, int height);
    void bind(unsigned int fbo);
    void use(size_t index) {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_CUBE_MAP, map);
    }
private: 
    unsigned int map;
};

#endif