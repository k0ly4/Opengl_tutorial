#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource/FileManager.h"
/// <summary>
/// TextureCubeMap
/// </summary>
class TextureCubeMap:public iTexture,public Sizeable {

public:

    TextureCubeMap() {}

    bool loadFromDirectory(const std::string& directory, bool flipVertically = 1, bool gammaMod = 0);

    void create(const glm::ivec2& size, const TextureFormatData& format);
    inline void create(const glm::ivec2& size, GLenum internal_format, GLenum format) { create(size, TextureFormatData(internal_format, format));}

    void wrap(unsigned S, unsigned T, unsigned R) {
        if (wrap_.get() == glm::uvec3(S, T, R))
            return;
        sTexture::bindCubeMap(id_);
        wrap_.setup(GL_TEXTURE_CUBE_MAP, S, T, R);
    }

    inline void wrap(unsigned STR) {
        wrap(STR, STR, STR);
    }

    void filter(unsigned mag, unsigned min){
        if (filter_.max == mag && filter_.min == min)
            return;
        sTexture::bindCubeMap(id_);
        filter_.setup(GL_TEXTURE_CUBE_MAP, mag, min);
    }

    void filter(unsigned newFilter) {
        filter(newFilter, newFilter);
    }

    inline void use(unsigned int text_unit) const {
        sTexture::active(GL_TEXTURE0 + text_unit);
        sTexture::bindCubeMap(id_);
    }

    void bindToFramebuffer();
private:

    bool detach() {
        if (id_.remake()) {
            sTexture::bindCubeMap(id_);
            wrap_.setup(GL_TEXTURE_CUBE_MAP);
            filter_.setup(GL_TEXTURE_CUBE_MAP);
            return 1;
        }
        sTexture::bindCubeMap(id_);
        return 0;
    }

    tFilter filter_;
    tWrap3D wrap_;
};

/// <summary>
/// Texture2D
/// </summary>
class Texture2DLite:public iTexture {
public:

    Texture2DLite(TextureId id, const glm::ivec2& size) :
        iTexture(id),
        size_(size) {}
    Texture2DLite() :size_(0) {}
    const glm::ivec2& getSize()const { return size_; }
    inline void use(size_t text_unit)const {
        sTexture::active(GL_TEXTURE0 + text_unit);
        sTexture::bind2D(id_);
    }
   
private:
    glm::ivec2 size_;
};
/// <summary>
/// Texture2D
/// </summary>
class Texture2D {

public:

    enum Channels :int
    {
        unlimited = -1,
    };

    Texture2D() { resource = std::make_shared<ResourceTexture2D>(); }

    Texture2D(std::shared_ptr <ResourceTexture2D> res) : resource(res) {}
    Texture2D(const glm::ivec2& size, const TextureFormatData& format, const void* data = nullptr, int sizeMipmaps = unlimited) :
        resource(std::make_shared<ResourceTexture2D>(size, format, data, sizeMipmaps)) {}

    inline bool load(const std::string& path_to_image, int sizeMipmaps = unlimited, bool gammaMod = 1) {
        return create(ImageLoader::getTex2D(path_to_image, gammaMod), sizeMipmaps);
    }
    //ImageLoader::loadTex2D 
    inline bool qLoad(const std::string& path_to_image, int sizeMipmaps = unlimited, bool gammaMod = 1) {
        return create(ImageLoader::loadTexture2D(path_to_image, gammaMod), sizeMipmaps);
    }

    inline void create(const glm::ivec2& size, const TextureFormatData& format, const void* data = nullptr, int sizeMipmaps = unlimited) {
        create(std::make_shared<ResourceTexture2D>(size, format, data, sizeMipmaps));
    }
    void bindToFramebuffer(size_t unit);
    inline void use(size_t unit)const noexcept { resource->use(unit); }
    std::shared_ptr <ResourceTexture2D> resource;

private:
    inline bool create(std::shared_ptr <ResourceTexture2D> tmp) {
        if (tmp == 0) return 0;
        tmp->setFilter(resource->getFilter());
        tmp->setWrap(resource->getWrap());
        resource = tmp;
        return 1;
    }
    inline bool create(std::shared_ptr <ResourceTexture2D> tmp, int sizeMipmaps) {
        if (tmp == 0) return 0;
        tmp->setFilter(resource->getFilter());
        tmp->setWrap(resource->getWrap());
        resource = tmp;
        resource->genMipmaps(sizeMipmaps);
        return 1;
    }

};

/// <summary>
/// ArrayTexture2D
/// </summary>
class ArrayTexture2D:public iTexture, public Sizeable {

public:

    ArrayTexture2D():
        count_(0)
    {}

    void create(size_t count, const glm::ivec2& size, const TextureFormatData& format, const void* data = nullptr);

    inline void create(size_t count, const glm::ivec2& size, GLenum internal_format, GLenum format, const void* data = nullptr) {
        create(count, size, TextureFormatData(internal_format, format), data);
    }

    inline void use(size_t text_unit)const {
        sTexture::active(GL_TEXTURE0 + text_unit);
        sTexture::bind2DArray(id_);
    }

    void wrap(const tWrap2D& _wrap_) {
        if (wrap_== _wrap_) return;
        wrap_ = _wrap_;
        sTexture::bind2DArray(id_);
        sTexture::parameter(GL_TEXTURE_2D_ARRAY, wrap_);
    }

    inline void wrap(const tWrap2D& _wrap_, const glm::vec4& border_color) {
        wrap(_wrap_);
        wrap_.setBorderColor(GL_TEXTURE_2D_ARRAY, border_color);
    }

    inline void filter(GLint filter_min, GLint filter_max) {
        if (filter_.max == filter_max && filter_.min == filter_min)  return;
        sTexture::bind2DArray(id_);
        filter_.setup(GL_TEXTURE_2D_ARRAY, filter_max, filter_min);
    }
    inline void filter(GLint filterMinMax) { filter(filterMinMax, filterMinMax); }

    inline size_t getCount()const { return count_; }

private:

    bool detach() {
        if (id_.remake()) {
            sTexture::bind2DArray(id_);
            sTexture::parameter(GL_TEXTURE_2D_ARRAY, wrap_);
            sTexture::parameter(GL_TEXTURE_2D_ARRAY, filter_);
            return 1;
        }
        sTexture::bind2DArray(id_);
        return 0;
    }

    size_t count_;
    tFilter filter_;
    tWrap2D wrap_;
};
#endif