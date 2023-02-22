
#include "Framebuffer.h"  

inline bool testSize(const char* nameClass,const glm::ivec2& size)noexcept {

    if (size.x == 0 || size.y == 0) {
        LOG(LogWarning, "%s::size.xy == 0\n", nameClass);
        return 1;
    }

    return 0;
}
/// <summary>
/// FrameBuffer
/// </summary>


bool FrameBuffer::setSize(const glm::ivec2& size) {

    if (testSize("FrameBuffer", size)) return 0;

    size_ = size;
    std::vector<GLenum> attachment(texturesFormat_.size());
    sRender::Framebuffer::bind(*this, 0);

    for (size_t i = 0; i < texturesFormat_.size(); i++) {
        sTexture::bind2D(textures_[i]);

        texturesFormat_[i].setDataTexture2D(size_, NULL);

        attachment[i] = GL_COLOR_ATTACHMENT0 + i;
        sRender::Framebuffer::bindTexture2D(attachment[i], textures_[i]);
    }
    sRender::setDrawBuffers(attachment);
    sRender::RenderBuffer::bind(RBO);
    sRender::RenderBuffer::storage(size_);
    sRender::RenderBuffer::bindToFramebuffer(RBO);

    if(sRender::Framebuffer::checkStatus("FrameBuffer::Non complete\n")) return 0;
    sRender::Framebuffer::unbind();
    return 1;
}

bool FrameBuffer::create(const glm::ivec2& size, size_t sizeTextures, const TextureData* setupTextures)
{
        if (testSize("RenderTexture", size)) return 0;

        size_ = size;
        std::vector<unsigned int> attachment(sizeTextures);
        textures_.clear();
        textures_.reserve(sizeTextures);
        texturesFormat_ = std::vector<TextureData>(setupTextures, setupTextures + sizeTextures);

        sRender::Framebuffer::bind(*this,0);
        for (size_t i = 0; i < sizeTextures; i++) {

            textures_.push_back({});
            sTexture::bind2D(textures_[i]);

            texturesFormat_[i].setDataTexture2D(size_, NULL);
            texturesFormat_[i].setParameteriTexture2D();

            attachment[i] = GL_COLOR_ATTACHMENT0 + i;
            sRender::Framebuffer::bindTexture2D(attachment[i], textures_[i]);
        }
        sRender::setDrawBuffers(attachment);
        sRender::RenderBuffer::bind(RBO);
        sRender::RenderBuffer::storage(size_);
        sRender::RenderBuffer::bindToFramebuffer(RBO);

        if (sRender::Framebuffer::checkStatus("FrameBuffer::Non complete\n")) return 0;
        sRender::Framebuffer::unbind();
        return 1;
}

/// <summary>
/// RenderColor
/// </summary>
bool RenderColor::create(const glm::ivec2& size, size_t sizeTextures, const TextureData* setupTextures)
{   
    if (testSize("RenderColor", size))
        return 0;
    size_ = size;
    std::vector<unsigned int> attachment(sizeTextures);
    textures_.clear();
    textures_.reserve(sizeTextures);

    sRender::Framebuffer::bind(*this,0);
    for (size_t i = 0; i < sizeTextures; i++) {
        textures_.push_back({});
        sTexture::bind2D(textures_[i]);

        setupTextures[i].setDataTexture2D(size_, NULL);
        setupTextures[i].setParameteriTexture2D();

        attachment[i] = GL_COLOR_ATTACHMENT0 + i;
        sRender::Framebuffer::bindTexture2D(attachment[i], textures_[i]);
    }
    sRender::setDrawBuffers(attachment);
    if (sRender::Framebuffer::checkStatus("RenderColor::Non complete\n"))return 0;
    sRender::Framebuffer::unbind();
    return 1;
}

/// <summary>
/// RenderTexture
/// </summary>
//TextureFormatData data_format_;
bool RenderTexture::create(const glm::ivec2& size, const TextureFormatData& dFormat,tFilter filter, tWrap2D wrap) {
        //test 
        if (testSize("RenderTexture",size)) return 0;
        if (texture_ != 0 && texture_->resource->size_ == size)return 0;
        //texture
        createTexture(size, dFormat, filter, wrap);
        //bind
        return bind();
}

bool RenderTexture::create(const glm::ivec2& size) {
    //test
    testSize("RenderTexture", size);
    if (texture_ != 0 && texture_->resource->size_ == size  )return 0;
   //texture
    if (texture_ == 0) 
        createTexture(size, TextureFormatData(GL_RGBA, GL_RGBA), tFilter::Nearest, tWrap::ClampToEdge);
    else 
        createTexture(size, TextureFormatData(GL_RGBA, GL_RGBA), texture_->resource->getFilter(), texture_->resource->getWrap());
   //bind
    return bind();
}