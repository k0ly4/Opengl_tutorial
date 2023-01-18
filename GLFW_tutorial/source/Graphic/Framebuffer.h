#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Resource/buffer.h"
#include "Graphic/Render.h"
#include "Graphic/TextureEntity.h"
/// <summary>
/// NonCopyable
/// </summary>
class NonCopyable {
    protected:
          NonCopyable() {} 
          virtual ~NonCopyable() {}
    private:
            NonCopyable(const NonCopyable&) = delete;
            NonCopyable & operator =(const NonCopyable&) = delete;
};
/// <summary>
/// FrameBuffer
/// </summary>
class FrameBuffer :public iFrame,public Sizeable {
    
public:
    FrameBuffer() {
        glGenFramebuffers(1, &id_);
        glGenRenderbuffers(1, &rendereBuffer_);
    }

    Texture2DLite getTexture(size_t index = 0) { return { textures_[index], size_ }; }
    /// <summary>
    /// create
    /// </summary>
    bool create(const glm::ivec2& render_size, size_t sizeTextures, const TextureData* setupTextures);
    bool create(int width, int height, size_t sizeTextures, const TextureData* setupTexture) {
        return create(glm::ivec2(width, height), sizeTextures, setupTexture);
    }
    bool create(const glm::ivec2& render_size, const std::vector<TextureData>& textures) {
        return create(render_size, textures.size(), textures.data());
    }
    bool create(const glm::ivec2& render_size,const TextureData& texture_format) {
        return create(render_size, 1, &texture_format);
    }

    bool setSize(const glm::ivec2& render_size);
    const glm::ivec2& size()const { return size_; }

    void implementDepth(unsigned int write_fbo);
    inline void implementDepth(FrameBuffer& write_fbo) { implementDepth(write_fbo.id_);}

    ~FrameBuffer() {
        glDeleteRenderbuffers(1, &rendereBuffer_);
        glDeleteFramebuffers(1, &id_);
    }

protected:

    unsigned int rendereBuffer_;
    std::vector<TexPtr> textures_;
    std::vector<TextureData> texturesFormat_;
    
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
};

/// <summary>
/// RenderColor
/// </summary>
class RenderColor :public iFrame {
 
public:

    RenderColor() {
        Render::gen(*this);
    }

    bool create(const glm::ivec2& render_size, size_t sizeTextures, const TextureData* setupTextures);
    bool create(int width, int height, size_t sizeTextures, const TextureData* setupTexture) {
        return create(glm::ivec2(width, height), sizeTextures, setupTexture);
    }
    bool create(const glm::ivec2& render_size, const std::vector<TextureData>& textures){
        return create(render_size, textures.size(), textures.data());
    }
    Texture2DLite getTexture(size_t index = 0) { return { textures_[index],size_ }; }
    const glm::ivec2& size()const { return size_; }
    ~RenderColor() { Render::free(*this); }

private:
    glm::uvec2 size_;
    std::vector<TexPtr> textures_;

    RenderColor(const RenderColor&) = delete;
    RenderColor& operator=(const RenderColor&) = delete;
};

/// <summary>
/// RenderTexture
/// </summary>
class RenderTexture:public iFrame,public RenderTarget,public NonCopyable {

public:
    RenderTexture(){    Render::gen(*this);    }
    inline bool bind(std::shared_ptr<Texture2D> texture) {
        texture_ = texture;
        return bind();
    }
    bool create(const glm::ivec2& size);
    bool create(
        const glm::ivec2& size, 
        const TextureFormatData& dFormat,
        tFilter filter = tFilter(   tFilter::Nearest),
        tWrap2D wrap = tWrap2D(     tWrap2D::ClampToEdge)
    );
    bool create(int width, int height, const TextureFormatData& dFormat) { return create(glm::ivec2(width, height), dFormat);}
   
    inline std::shared_ptr<Texture2D>& texture() {  return texture_;}
    inline const glm::ivec2& size()const {          return texture_->getSize(); }
    virtual ~RenderTexture() {                      Render::free(*this); }

private:

    inline bool bind() {
        Render::bind(*this, 0);
        texture_->bindToFramebuffer(0);
        if (Render::checkFramebufferStatus("RenderTexture::Non complete\n"))return 0;
        Render::unbind();
        return 1;
    }
    inline void createTexture(const glm::ivec2& size,const TextureFormatData& format, const tFilter& filter, const tWrap2D& wrap) {
        texture_ = std::make_shared<Texture2D>(size, format);
        texture_->filter(filter);
        texture_->wrap(wrap);
    }

    std::shared_ptr<Texture2D> texture_ = 0;

};
#endif