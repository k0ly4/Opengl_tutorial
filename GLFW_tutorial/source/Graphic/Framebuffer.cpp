
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

void FrameBuffer::implementDepth(unsigned int write_fbo) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, write_fbo);
    glBlitFramebuffer(0, 0, size_.x, size_.y, 0, 0, size_.x, size_.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

bool FrameBuffer::setSize(const glm::ivec2& size) {

    if (testSize("FrameBuffer", size))
        return 0;

    size_ = size;
    std::vector<unsigned int> attachment(texturesFormat_.size());
    Render::bind(*this,0);

    for (size_t i = 0; i < texturesFormat_.size(); i++) {
        glTexture::bind2D(textures_[i]);

        texturesFormat_[i].setDataTexture2D(size_, NULL);

        attachment[i] = GL_COLOR_ATTACHMENT0 + i;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, textures_[i].id(), 0);

    }

    glDrawBuffers(attachment.size(), attachment.data());

    glBindRenderbuffer(GL_RENDERBUFFER, rendereBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size_.x, size_.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rendereBuffer_);

    if(Render::checkFramebufferStatus()) {
        LOG(LogError,"FrameBuffer::Non complete\n");
        return 0;
    }

    Render::unbind();
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

        Render::bind(*this,0);
        for (size_t i = 0; i < sizeTextures; i++) {

            textures_.push_back({});
            glTexture::bind2D(textures_[i]);

            texturesFormat_[i].setDataTexture2D(size_, NULL);
            texturesFormat_[i].setParameteriTexture2D();

            attachment[i] = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, textures_[i].id(), 0);
        }
        glDrawBuffers(attachment.size(), attachment.data());

        glBindRenderbuffer(GL_RENDERBUFFER, rendereBuffer_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size_.x, size_.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rendereBuffer_);

        if (Render::checkFramebufferStatus()) {
            LOG(LogError,"FrameBuffer::Non complete\n");
            return 0;
        }
        Render::unbind();
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

    Render::bind(*this,0);
    for (size_t i = 0; i < sizeTextures; i++) {
        textures_.push_back({});
        glTexture::bind2D(textures_[i]);

        setupTextures[i].setDataTexture2D(size_, NULL);
        setupTextures[i].setParameteriTexture2D();

        attachment[i] = GL_COLOR_ATTACHMENT0 + i;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, textures_[i].id(), 0);
    }
        glDrawBuffers(attachment.size(), attachment.data());

    if (Render::checkFramebufferStatus("RenderColor::Non complete\n"))return 0;

    Render::unbind();
    return 1;
}

/// <summary>
/// RenderTexture
/// </summary>
//TextureFormatData data_format_;
bool RenderTexture::create(const glm::ivec2& size, const TextureFormatData& dFormat,tFilter filter, tWrap2D wrap) {
        //test 
        if (testSize("RenderTexture",size)) return 0;
        if (texture_ != 0 && texture_->getSize() == size)return 0;
        //texture
        createTexture(size, dFormat, filter, wrap);
        //bind
        return bind();
}

bool RenderTexture::create(const glm::ivec2& size) {
    //test
    testSize("RenderTexture", size);
    if (texture_ != 0 && texture_->getSize() == size  )return 0;
   //texture
    if (texture_ == 0) 
        createTexture(size, TextureFormatData(GL_RGBA, GL_RGBA), tFilter::Nearest, tWrap::ClampToEdge);
    else 
        createTexture(size, TextureFormatData(GL_RGBA, GL_RGBA), texture_->resource()->getFilter(), texture_->resource()->getWrap());
   //bind
    return bind();
}