#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource/Resource_manager.h"

/// <summary>
/// TextureCubeMap
/// </summary>
class TextureCubeMap:public Texture, public Sizeable {

public:

    TextureCubeMap() :
        Sizeable() {}

    bool loadFromDirectory(const std::string& directory, bool flipVertically = 1, bool gammaMod = 0);

    void create(const glm::ivec2& size, const TextureFormat& format);
    inline void create(const glm::ivec2& size, GLenum internal_format, GLenum format) {
        create(size, TextureFormat(internal_format, format));
    }

    void wrap(unsigned S, unsigned T, unsigned R) {
        if (wrap_.get() == glm::uvec3(S, T, R))
            return;
        glTexture::bindCubeMap(id_);
        wrap_.setup(GL_TEXTURE_CUBE_MAP, S, T, R);
    }

    inline void wrap(unsigned STR) {
        wrap(STR, STR, STR);
    }

    void filter(unsigned mag, unsigned min){
        if (filter_.getMax() == mag && filter_.getMin() == min)
            return;
        glTexture::bindCubeMap(id_);
        filter_.setup(GL_TEXTURE_CUBE_MAP, mag, min);
    }

    void filter(unsigned newFilter) {
        filter(newFilter, newFilter);
    }

    inline void use(unsigned int text_unit) const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bindCubeMap(id_);
    }

    inline void bindToFramebuffer() {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id_.get(), 0);
    }
private:

    bool detach() {
        if (id_.reBuild()) {
            glTexture::bindCubeMap(id_);
            wrap_.setup(GL_TEXTURE_CUBE_MAP);
            filter_.setup(GL_TEXTURE_CUBE_MAP);
            return 1;
        }
        glTexture::bindCubeMap(id_);
        return 0;
    }

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
class Texture2D:public GeneralTexture2D,public Sizeable {

public:

    Texture2D():
        sizeMipmaps_(0),
        Sizeable()
    {}

    Texture2D(const TexturePointer& textureId, const glm::ivec2& size):
        Sizeable(size),
        GeneralTexture2D(textureId),
        sizeMipmaps_(0)
    {}

    Texture2D(const TextureResource&resource):
        Texture2D(resource.getId(),resource.getSize())
    {}

    Texture2D(const TexturePointer& textureId, int width, int height) :
        Texture2D(textureId, glm::ivec2(width, height))
    {}

    bool getPath(const std::string& path_to_image, int sizeMipmaps = -1, bool gammaMod = 1);

    void create(const glm::ivec2& size,const TextureFormat& format, const void* data = nullptr, int sizeMipmaps = -1);

    inline void create(const glm::ivec2& size, GLint internal_format, GLint format, const void* data = nullptr, int sizeMipmaps = -1) {
        create(size, TextureFormat(internal_format, format), data, sizeMipmaps);
    }

    void wrap(GLint S, GLint T) {
        if (wrap_.get() == glm::uvec2(S, T))
            return;
        glTexture::bind2D(id_);
        wrap_.setup(GL_TEXTURE_2D, S, T);
    }

    inline void wrap(GLint ST) {
        wrap(ST, ST);
    }

    void filter( GLint filter_max, GLint filter_min) {
        if (filter_.getMax() == filter_max && filter_.getMin() == filter_min)
            return;
        glTexture::bind2D(id_);
        filter_.setup(GL_TEXTURE_2D, filter_max, filter_min);
    }
    
    inline void filter(GLint filterMinMax) {
        filter(filterMinMax, filterMinMax);
    }

    inline void filter(const TextureFilter& filterMinMax) {
        filter(filterMinMax.getMax(), filterMinMax.getMin());
    }

    inline int getSizeMipmaps()const  {
        return sizeMipmaps_;
    }

    inline void bindToFramebuffer(size_t unit) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + unit, GL_TEXTURE_2D, id_.get(), 0);
    }
private:

    bool detach() {
        if (id_.reBuild()) {
            glTexture::bind2D(id_);
            wrap_.setup(GL_TEXTURE_2D);
            filter_.setup(GL_TEXTURE_2D);
            return 1;
        }
        glTexture::bind2D(id_);
        return 0;  
    }

    int sizeMipmaps_;
    TextureFilter filter_;
    TextureWrap2D wrap_;
};

/// <summary>
/// ArrayTexture2D
/// </summary>
class ArrayTexture2D:public Texture,public Sizeable {

public:

    ArrayTexture2D()
        :Sizeable(),
        count_(0)
    {}

    void create(size_t count, const glm::ivec2& size, const TextureFormat& format, const void* data = nullptr);

    inline void create(size_t count, const glm::ivec2& size, GLenum internal_format, GLenum format, const void* data = nullptr) {
        create(count, size, TextureFormat(internal_format, format), data);
    }

    inline void use(size_t text_unit)const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bind2DArray(id_);
    }

    void wrap(GLint wrap_S, GLint wrap_T) {
        if (wrap_.get() == glm::uvec2(wrap_S, wrap_T)) 
            return;

        glTexture::bind2DArray(id_);
        wrap_.setup(GL_TEXTURE_2D_ARRAY, wrap_S, wrap_T);
    }
    inline void wrap(GLint ST) {
        wrap(ST, ST);
    }
    inline void wrap(GLint wrap_S, GLint wrap_T, const glm::vec4& border_color) {
        wrap(wrap_S, wrap_T);
        wrap_.setBorderColor(GL_TEXTURE_2D_ARRAY, border_color);
    }

    void filter(GLint filter_min, GLint filter_max) {
        if (filter_.getMax() == filter_max && filter_.getMin() == filter_min) 
            return;

        glTexture::bind2DArray(id_);
        filter_.setup(GL_TEXTURE_2D_ARRAY, filter_max, filter_min);
    }
    inline void filter(GLint filterMinMax) {
        filter(filterMinMax, filterMinMax);
    }

    inline size_t getCount()const {
        return count_;
    }

private:

    bool detach() {
        if (id_.reBuild()) {
            glTexture::bind2DArray(id_);
            wrap_.setup(GL_TEXTURE_2D_ARRAY);
            filter_.setup(GL_TEXTURE_2D_ARRAY);
            return 1;
        }
        glTexture::bind2DArray(id_);
        return 0;
    }

    size_t count_;
    TextureFilter filter_;
    TextureWrap2D wrap_;
};

#endif