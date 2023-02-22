#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Resource/buffer.h"
#include "Graphic/Render.h"
#include "Graphic/TextureEntity.h"
#include "Graphic/Texture.h"
#include "Graphic/Drawable.h"
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
class FrameBuffer :public iFramebuffer, public Sizeable {

public:
    FrameBuffer() {}
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
    bool create(const glm::ivec2& render_size, const TextureData& texture_format) {
        return create(render_size, 1, &texture_format);
    }

    bool setSize(const glm::ivec2& render_size);
    const glm::ivec2& size()const { return size_; }

    
    inline void implementDepth(unsigned int write_fbo)     { sRender::Framebuffer::blit(id_.get(), size_, write_fbo, size_, sRender::DEPTH_BIT); }
    inline void implementDepth(FrameBuffer& write_fbo) { implementDepth(write_fbo.id());}

protected:

    RenderbufferId RBO;
    std::vector<TextureId> textures_;
    std::vector<TextureData> texturesFormat_;
    
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
};

/// <summary>
/// RenderColor
/// </summary>
class RenderColor :public iFramebuffer {
 
public:

    bool create(const glm::ivec2& render_size, size_t sizeTextures, const TextureData* setupTextures);
    bool create(int width, int height, size_t sizeTextures, const TextureData* setupTexture) {
        return create(glm::ivec2(width, height), sizeTextures, setupTexture);
    }
    bool create(const glm::ivec2& render_size, const std::vector<TextureData>& textures){
        return create(render_size, textures.size(), textures.data());
    }
    Texture2DLite getTexture(size_t index = 0) { return { textures_[index],size_ }; }
    const glm::ivec2& size()const { return size_; }

private:
    glm::uvec2 size_;
    std::vector<TextureId> textures_;

    RenderColor(const RenderColor&) = delete;
    RenderColor& operator=(const RenderColor&) = delete;
};

/// <summary>
/// RenderTexture
/// </summary>
class RenderTexture:public iFramebuffer,public RenderTarget,public NonCopyable {

public:
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
   
    inline std::shared_ptr<Texture2D>& texture()            {   return texture_;}
    inline const std::shared_ptr<Texture2D>& texture()const {   return texture_; }
    inline const glm::ivec2& size()const {                      return texture_->resource->size_; }

private:

    inline bool bind() {
        sRender::Framebuffer::bind(*this, 0);
        texture_->bindToFramebuffer(0);
        if (sRender::Framebuffer::checkStatus("RenderTexture::Non complete\n"))return 0;
        sRender::Framebuffer::unbind();
        return 1;
    }
    inline void createTexture(const glm::ivec2& size,const TextureFormatData& format, const tFilter& filter, const tWrap2D& wrap) {
        texture_ = std::make_shared<Texture2D>(size, format);
        texture_->resource->setFilter(filter);
        texture_->resource->setWrap(wrap);
    }

    std::shared_ptr<Texture2D> texture_ = 0;

};
#endif