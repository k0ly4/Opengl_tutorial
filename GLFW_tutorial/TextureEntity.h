#ifndef TEXTURE_ENTITY
#define TEXTURE_ENTITY

#include "Shader.h"
#include "Resource_manager.h"
#include <list>
#include "Exception.h"


class TextureCubeMap;
class GeneralTexture2D;
class Texture2D;
class ArrayTexture2D;
class TextureCubeDepth;

/// <summary>
/// TextureFilter
/// </summary>
class TextureFilter {
public:

    enum Mode : unsigned int
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,

    };

    TextureFilter() :
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
    unsigned getMin()const {
        return filter_min_;
    }
    unsigned getMax()const {
        return filter_max_;
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
class TextureWrap3D :public TextureWrap {

public:

    TextureWrap3D() :
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
class TextureWrap2D :public TextureWrap {
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
/// Sizeable
/// </summary>
/// 
class Sizeable {
public:

	Sizeable() :size_(0.f) {}
	Sizeable(const glm::ivec2& size) :size_(size) {}

	const glm::ivec2& getSize()const {
		return size_;
	}

protected:

	glm::ivec2 size_;

};

#endif

