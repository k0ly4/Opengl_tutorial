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
inline bool operator ==(const tFilter& l, const tFilter& r) { return (l.vec == r.vec); }
inline bool operator !=(const tFilter& l, const tFilter& r) { return (l.vec != r.vec); }
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

    virtual void setup(GLenum target)const = 0;
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

    void setup(GLenum target)const {
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

    inline void setup(GLenum target)const {
        glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
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
inline bool operator ==(const tWrap2D& l, const tWrap2D& r) { return (l.vec == r.vec); }
inline bool operator !=(const tWrap2D& l, const tWrap2D& r) { return (l.vec != r.vec); }
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
/// textureId
/// </summary>
class TextureId {
/// <summary>
/// ID
/// </summary>
    struct ID {
        unsigned int id;
        ID() {          sTexture::gen(id); }
        virtual ~ID() { sTexture::free(id); }
    };

public:

    TextureId():id_(std::make_shared<ID>()){}
    inline bool remake()noexcept {
        if (id_.unique() == 0) {
            id_ = std::make_shared<ID>();
            return 1;
        }
        return 0;
    }
    inline unsigned int get()const noexcept { return id_->id; }
private:

    std::shared_ptr<ID> id_ =0;
};
// <summary>
/// TextureDataFormat
/// </summary>
struct TextureData
{
    TextureData(const TextureFormatData& format, const tFilter& filter) :
        format_(format),
        filter_(filter)
    {}

    TextureData(GLenum internal_format, GLenum format, GLenum filter) :
        format_(internal_format, format),
        filter_(filter)
    {}

    TextureData(GLenum internal_format, GLenum format, GLenum filter_max, GLenum filter_min) :
        format_(internal_format, format),
        filter_(filter_max, filter_min)
    {}
    void setParameteriTexture2D()const {
        filter_.setup(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    inline void setDataTexture2D(const glm::ivec2& size, const void* data)const {
        sTexture::dataImage2D(size, format_, data);
    }

    inline const TextureFormatData& getFormat()const { return format_; }

    inline const tFilter& getFilter()const { return filter_; }
private:

    TextureFormatData   format_;
    tFilter       filter_;

};

/// <summary>
/// Texture
/// </summary>
class iTexture {

public:

    iTexture(const TextureId& id) :id_(id) {}
    iTexture() {}
    const TextureId& getId()const {    return id_; }
    virtual inline void remakeId() {     id_.remake();}

protected:
    TextureId id_;
};

#endif

