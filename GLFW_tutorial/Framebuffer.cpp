#include "Framebuffer.h"
#include "Exception.h"
    
/// <summary>
/// FrameBuffer
/// </summary>

void FrameBuffer::implementDepth(unsigned int write_fbo) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, write_fbo);
    glBlitFramebuffer(0, 0, size_.x, size_.y, 0, 0, size_.x, size_.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

bool FrameBuffer::setSize(const glm::ivec2& render_size) {
    size_ = render_size;
    std::vector<unsigned int> attachment(texturesFormat_.size());
    GlRender::bind(*this,0);

    for (size_t i = 0; i < texturesFormat_.size(); i++) {
        glTexture::bind2D(textures_[i]);

        texturesFormat_[i].setDataTexture2D(size_, NULL);

        attachment[i] = GL_COLOR_ATTACHMENT0 + i;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, textures_[i].get(), 0);

    }

    glDrawBuffers(attachment.size(), attachment.data());

    glBindRenderbuffer(GL_RENDERBUFFER, rendereBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size_.x, size_.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rendereBuffer_);

    if(GlRender::checkFramebufferStatus()) {
        log("(!)Error::FrameBuffer::Non complete\n");
        return 0;
    }

    GlRender::unbind();
    return 1;
}

bool FrameBuffer::create(const glm::ivec2& render_size, size_t sizeTextures, const TextureData* setupTextures)
{
   
        size_ = render_size;
        std::vector<unsigned int> attachment(sizeTextures);
        textures_.clear();
        textures_.reserve(sizeTextures);
        texturesFormat_ = std::vector<TextureData>(setupTextures, setupTextures + sizeTextures);

        GlRender::bind(*this,0);
        for (size_t i = 0; i < sizeTextures; i++) {

            textures_.push_back({});
            glTexture::bind2D(textures_[i]);

            texturesFormat_[i].setDataTexture2D(size_, NULL);
            texturesFormat_[i].setParameteriTexture2D();

            attachment[i] = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, textures_[i].get(), 0);
        }
        glDrawBuffers(attachment.size(), attachment.data());

        glBindRenderbuffer(GL_RENDERBUFFER, rendereBuffer_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size_.x, size_.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rendereBuffer_);

        if (GlRender::checkFramebufferStatus()) {
            log("(!)Error::FrameBuffer::Non complete\n");
            return 0;
        }
        GlRender::unbind();
        return 1;
}

/// <summary>
/// RenderColor
/// </summary>
bool RenderColor::create(const glm::ivec2& render_size, size_t sizeTextures, const TextureData* setupTextures)
{
    size_ = render_size;
    std::vector<unsigned int> attachment(sizeTextures);
    textures_.clear();
    textures_.reserve(sizeTextures);

    GlRender::bind(*this,0);
    for (size_t i = 0; i < sizeTextures; i++) {
        textures_.push_back({});
        glTexture::bind2D(textures_[i]);

        setupTextures[i].setDataTexture2D(size_, NULL);
        setupTextures[i].setParameteriTexture2D();

        attachment[i] = GL_COLOR_ATTACHMENT0 + i;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, textures_[i].get(), 0);
    }
        glDrawBuffers(attachment.size(), attachment.data());

    if (GlRender::checkFramebufferStatus()) {
        log("(!)Error::RenderColor::Non complete\n");
        return 0;
    }

    GlRender::unbind();
    return 1;
}

/// <summary>
/// RenderTexture
/// </summary>

bool RenderTexture::create(const glm::ivec2& size, const TextureData& format_data) {
        size_ = size;
        data_format_ = format_data.getFormat();
        
        texture.create(size_, data_format_);
        texture.filter(format_data.getFilter());
        texture.wrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        GlRender::bind(*this,0);
        texture.bindToFramebuffer(0);

        if (GlRender::checkFramebufferStatus()) {
            log("(!)Error::RenderTexture::Non complete\n");
            return 0;
        }   

        GlRender::unbind();
        return 1;
}

bool RenderTexture::create(const glm::ivec2& size) {

    if (size_ == size) return 1; 
  
    texture.create(size_, data_format_);
    GlRender::bind(*this, 0);
    texture.bindToFramebuffer(0);

    if (GlRender::checkFramebufferStatus()) {
        log("(!)Error::RenderTexture::Non complete\n");
        return 0;
    }

    GlRender::unbind();
    return 1;
}