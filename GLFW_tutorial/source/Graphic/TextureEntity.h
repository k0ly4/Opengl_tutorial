#ifndef TEXTURE_ENTITY
#define TEXTURE_ENTITY


#include <list>
#include "Math/Math.h"
#include "System/Exception.h"
#include "ContextTexture.h"
/// <summary>
/// TextureFilter
/// </summary>
class tFilter {
public:

    enum Mode : GLint
    {
        Nearest =               GL_NEAREST,
        Linear =                GL_LINEAR,
        NearestMipmapNearest =  GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapNearest =   GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmapLinear =   GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmapLinear =    GL_LINEAR_MIPMAP_LINEAR,

    };

    tFilter() :
        min(NearestMipmapLinear),
        max(Linear)
    {}
    tFilter(GLint filter_max, GLint filter_min) :
        min(filter_min),
        max(filter_max)
    {}
    tFilter(GLint filter) :
        tFilter(filter,filter)
    {}

    inline void set(GLint mag, GLint min) {
        min = min;
        max = mag;
    }

    inline void setup(GLenum target)const {
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, max);        
    }

    inline void setup(GLenum target, GLint mag, GLint min) {
        set(mag, min);
        setup(target);
    }
   union{
        struct {
            GLint min; // режим фильтрации, если пикселей текстуры < пикселей экрана
            GLint max; // режим фильтрации, если пикселей текстуры > пикселей экрана};
       };
        glm::ivec2 vec;
   };
   
};

/// <summary>
/// TextureWrap
/// </summary>
class tWrap {
public:
    enum Mode : unsigned int
    {
        ClampToEdge =       GL_CLAMP_TO_EDGE,
        ClampToBorder =     GL_CLAMP_TO_BORDER,
        MirroredRepeat =    GL_MIRRORED_REPEAT,
        Repeat =            GL_REPEAT,
    };

    inline void setBorderColor(GLenum target, const glm::vec4& borderColor) {
        glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
    }

    virtual void setup(GLenum target) = 0;
};

/// <summary>
/// TextureWrap3D
/// </summary>
class tWrap3D :public tWrap {

public:

    tWrap3D() :
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

    glm::uvec3 get()const { return { Wrap_S, Wrap_T, Wrap_R}; }

protected:
    // Конфигурация текстуры
    unsigned int Wrap_S; // режим наложения по оси S
    unsigned int Wrap_T; // режим наложения по оси T
    unsigned int Wrap_R; // режим наложения по оси R

};

/// <summary>
/// TextureWrap2D
/// </summary>
class tWrap2D :public tWrap {
public:

    tWrap2D() :
        s(Repeat),
        t(Repeat)
    {}
    tWrap2D(GLint S_, GLint T_) :
        s(S_),
        t(T_)
    {}
    tWrap2D(GLint ST_) :
        s(ST_),
        t(ST_)
    {}
    void set(GLint S_, GLint T_) {
        s = S_;
        t = T_;
    }

    void setup(GLenum target) {
        glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
    }

    void setup(GLenum target, GLint S_, GLint T_) {
        set(S_, T_);
        setup(target);
    }
    union 
    {
        struct { // Конфигурация текстуры
            GLint s; // режим наложения по оси S
            GLint t; // режим наложения по оси T
        };
        glm::ivec2 vec;
    };
   
};

/// <summary>
/// Sizeable
/// </summary>
/// 
class Sizeable {
public:

	Sizeable() :size_(0) {}
	Sizeable(const glm::ivec2& size) :size_(size) {}

	const glm::ivec2& getSize()const {
		return size_;
	}

protected:

	glm::ivec2 size_;

};

/// <summary>
/// TextureID
/// </summary>
class TextureID {
public:
    unsigned int id;
    TextureID() {           glTexture::gen(id); }
    virtual ~TextureID() {  glTexture::free(id); }
};

/// <summary>
/// TextureShell
/// </summary>
class TexPtr {

public:

    TexPtr() :texture(std::make_shared<TextureID>())
    {}

    inline bool make() {
        if (texture.unique() == 0) {
            texture = std::make_shared<TextureID>();
            return 1;
        }
        return 0;
    }

    inline unsigned int id() const { return texture->id;}

private:
    std::shared_ptr<TextureID> texture;
};

/// <summary>
/// TextureDataFormat
/// </summary>
class TextureFormatData
{
public:

    TextureFormatData(GLint internalformat_, GLint format_) :
        internalFormat(internalformat_),
        format(format_)
    {}

    inline void dataImage2D(GLenum target, const glm::ivec2& size, const void* data)const {
        glTexImage2D(
            target,
            0,
            internalFormat,
            size.x,
            size.y,
            0,
            format,
            getType(internalFormat),
            data);
    }
    inline void dataImage2D(const glm::ivec2& size, const void* data)const {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            internalFormat,
            size.x,
            size.y,
            0,
            format,
            getType(internalFormat),
            data);
    }
    inline void dataImage3D(GLenum target, const glm::ivec2& size, GLsizei depth, const void* data)const {
        glTexImage3D(
            target,
            0,
            internalFormat,
            size.x,
            size.y,
            depth,
            0,
            format,
            getType(internalFormat),
            data);
    }
    static GLenum getType(GLenum internal_format) {
        return internal_format == GL_RGBA16F || internal_format == GL_DEPTH_COMPONENT || internal_format == GL_DEPTH_COMPONENT32F ?
            GL_FLOAT : GL_UNSIGNED_BYTE;
    }
    GLint internalFormat, format;
private:
    
};

/// <summary>
/// TextureDataFormat
/// </summary>
struct TextureData
{
    TextureData(const TextureFormatData& format, const tFilter& filter) :
        format_(format),
        filter_(filter)
    {}

    TextureData(GLenum internal_format,GLenum format,GLenum filter):
        format_(internal_format,format),
        filter_(filter)
    {}

    TextureData(GLenum internal_format, GLenum format, GLenum filter_max, GLenum filter_min ) :
        format_(internal_format, format),
        filter_(filter_max,filter_min)
    {}
    void setParameteriTexture2D()const  {
        filter_.setup(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    inline void setDataTexture2D(const glm::ivec2& size,const void * data)const {
        format_.dataImage2D(size, data);
    }

    inline const TextureFormatData& getFormat()const {  return format_; }

    inline const tFilter& getFilter()const {            return filter_;}
private:

    TextureFormatData   format_;
    tFilter       filter_;

};

/// <summary>
/// Texture
/// </summary>
class iTexture {

public:

    iTexture(const TexPtr& id) :id_(id) {}
    iTexture() {}
    const TexPtr& getId()const {    return id_; }
    virtual inline bool detach() {  return id_.make();}

protected:
    TexPtr id_;
};

#endif

