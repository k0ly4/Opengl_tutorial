#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource/FileManager.h"

/// <summary>
/// TextureCubeMap
/// </summary>
class TextureCubeMap:public iTexture, public Sizeable {

public:

    TextureCubeMap() :
        Sizeable() {}

    bool loadFromDirectory(const std::string& directory, bool flipVertically = 1, bool gammaMod = 0);

    void create(const glm::ivec2& size, const TextureFormatData& format);
    inline void create(const glm::ivec2& size, GLenum internal_format, GLenum format) { create(size, TextureFormatData(internal_format, format));}

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
        if (filter_.max == mag && filter_.min == min)
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
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id_.id(), 0);
    }
private:

    bool detach() {
        if (id_.make()) {
            glTexture::bindCubeMap(id_);
            wrap_.setup(GL_TEXTURE_CUBE_MAP);
            filter_.setup(GL_TEXTURE_CUBE_MAP);
            return 1;
        }
        glTexture::bindCubeMap(id_);
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

    Texture2DLite(TexPtr id, const glm::ivec2& size) :
        iTexture(id),
        size_(size) {}
    Texture2DLite() :size_(0) {}
    const glm::ivec2& getSize()const { return size_; }
    inline void use(size_t text_unit)const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bind2D(id_);
    }
   
private:
    glm::ivec2 size_;
};
/// <summary>
/// Texture2D
/// </summary>
class Texture2D {

public:
    enum Channels:int
    {
        unlimited = -1,
    };
    Texture2D() : res_(0) {}
    Texture2D(std::shared_ptr <ResourceTexture2D> res): res_(res){}
    Texture2D(const glm::ivec2& size, const TextureFormatData& format, const void* data = nullptr, int sizeMipmaps = unlimited) :
        res_(std::make_shared<ResourceTexture2D>(size, format, data, sizeMipmaps)) {}
    inline bool load(const std::string& path_to_image, int sizeMipmaps = unlimited, bool gammaMod = 1) {
        res_ = ImageLoader::getTex2D(path_to_image, gammaMod);
        if (res_ == 0) return 0;
        res_->setMipmaps(sizeMipmaps);
        return 1;
    }
    //ImageLoader::loadTex2D 
    inline bool load_no_reg(const std::string& path_to_image, int sizeMipmaps = unlimited, bool gammaMod = 1) {
        res_ = ImageLoader::forwardLoadTex2D(path_to_image, gammaMod);
        if (res_ == 0) return 0;
        res_->setMipmaps(sizeMipmaps);
        return 1;
    }
    inline void create(const glm::ivec2& size, const TextureFormatData& format, const void* data = nullptr, int sizeMipmaps = unlimited) {
       res_ = std::make_shared<ResourceTexture2D>(size, format, data, sizeMipmaps);
    }

    inline void wrap(GLint S, GLint T) {
       if (res_ == 0)LOG(LogError, "Texture2D::wrap::res_==0\n");
       res_->wrap(S,T);
    }
    inline void filter(GLint filter_max, GLint filter_min) {
        if (res_ == 0)LOG(LogError, "Texture2D::filter::res_==0\n");
        res_->filter(filter_max, filter_min);
    }

    inline void wrap(GLint ST) {    wrap(ST, ST); }
    inline void wrap(const tWrap2D& w) {  wrap(w.s,w.t); }
    inline void filter(GLint filterMinMax) {                    filter(filterMinMax, filterMinMax);}
    inline void filter(const tFilter& filterMinMax) {           filter(filterMinMax.max, filterMinMax.min); }

    inline const glm::ivec2& getSize()const {  return res_->size_; }
    inline int getSizeMipmaps()const        {  return res_->sizeMipmaps(); }

    inline void bindToFramebuffer(size_t unit) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + unit, GL_TEXTURE_2D, res_->id_.id(), 0);
    }
    inline void use(size_t text_unit)const { res_->use(text_unit); }
    inline const std::shared_ptr <ResourceTexture2D>& resource()const { return res_;}
    inline bool isInit() const { return res_ !=0 ; }
private:
    std::shared_ptr <ResourceTexture2D> res_;
};

/// <summary>
/// ArrayTexture2D
/// </summary>
class ArrayTexture2D:public iTexture,public Sizeable {

public:

    ArrayTexture2D()
        :Sizeable(),
        count_(0)
    {}

    void create(size_t count, const glm::ivec2& size, const TextureFormatData& format, const void* data = nullptr);

    inline void create(size_t count, const glm::ivec2& size, GLenum internal_format, GLenum format, const void* data = nullptr) {
        create(count, size, TextureFormatData(internal_format, format), data);
    }

    inline void use(size_t text_unit)const {
        glTexture::active(GL_TEXTURE0 + text_unit);
        glTexture::bind2DArray(id_);
    }

    void wrap(GLint wrap_S, GLint wrap_T) {
        if (wrap_.vec == glm::ivec2(wrap_S, wrap_T)) return;

        glTexture::bind2DArray(id_);
        wrap_.setup(GL_TEXTURE_2D_ARRAY, wrap_S, wrap_T);
    }
    inline void wrap(GLint ST) { wrap(ST, ST); }
    inline void wrap(GLint wrap_S, GLint wrap_T, const glm::vec4& border_color) {
        wrap(wrap_S, wrap_T);
        wrap_.setBorderColor(GL_TEXTURE_2D_ARRAY, border_color);
    }

    inline void filter(GLint filter_min, GLint filter_max) {
        if (filter_.max == filter_max && filter_.min == filter_min)  return;
        glTexture::bind2DArray(id_);
        filter_.setup(GL_TEXTURE_2D_ARRAY, filter_max, filter_min);
    }
    inline void filter(GLint filterMinMax) { filter(filterMinMax, filterMinMax); }

    inline size_t getCount()const { return count_; }

private:

    bool detach() {
        if (id_.make()) {
            glTexture::bind2DArray(id_);
            wrap_.setup(GL_TEXTURE_2D_ARRAY);
            filter_.setup(GL_TEXTURE_2D_ARRAY);
            return 1;
        }
        glTexture::bind2DArray(id_);
        return 0;
    }

    size_t count_;
    tFilter filter_;
    tWrap2D wrap_;
};
#endif