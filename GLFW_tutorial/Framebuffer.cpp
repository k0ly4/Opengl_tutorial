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

    void FrameBuffer::setSize(const glm::ivec2& render_size) {
        try
        {
        size_ = render_size;
        std::vector<unsigned int> attachment(texturesFormat_.size());
        GlRender::bind(*this);

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

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw FramebufferNonCompleteException();
        }
    }
    catch (const Exception& exception)
    {
        exception.log();
    }

    GlRender::unbind();
    }

    void FrameBuffer::create(const glm::ivec2& render_size, size_t sizeTextures, const TextureDataFormat* setupTextures)
    {
        try
        {
            size_ = render_size;
            std::vector<unsigned int> attachment(sizeTextures);
            textures_.clear();
            textures_.reserve(sizeTextures);
            texturesFormat_ = std::vector<TextureDataFormat>(setupTextures, setupTextures + sizeTextures);

            GlRender::bind(*this);
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

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                throw FramebufferNonCompleteException();
            }
        }
        catch (const Exception& exception)
        {
           exception.log();
        }   

        GlRender::unbind();
    }

    /// <summary>
    /// RenderColor
    /// </summary>
    void RenderColor::create(const glm::ivec2& render_size, size_t sizeTextures, const TextureDataFormat* setupTextures)
    {
        try
        {
        size_ = render_size;
        std::vector<unsigned int> attachment(sizeTextures);
        textures_.clear();
        textures_.reserve(sizeTextures);

        GlRender::bind(*this);
        for (size_t i = 0; i < sizeTextures; i++) {
            textures_.push_back({});
            glTexture::bind2D(textures_[i]);

            glTexImage2D(GL_TEXTURE_2D, 0, setupTextures[i].internal_format, size_.x, size_.y, 0, setupTextures[i].format,
                setupTextures[i].internal_format == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, setupTextures[i].filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, setupTextures[i].filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            attachment[i] = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, textures_[i].get(), 0);
        }
            glDrawBuffers(attachment.size(), attachment.data());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw FramebufferNonCompleteException();
        }

        }

        catch (const Exception& exception)
        {
            exception.log();
        }

        GlRender::unbind();
    }

/// <summary>
/// RenderTexture
/// </summary>

    bool RenderTexture::create(int width, int height, GLint format, GLint filterMin, GLint filterMax) {
        
            size_ = glm::ivec2(width, height);
            texture.filter(filterMin, filterMax);
            texture.wrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

            texture.create(size_, format, format);
            glBindFramebuffer(GL_FRAMEBUFFER, id_);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getId().get(), 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                log("(!)Error::RenderTexture::Non complete\n");
                return 0;
            }   
        GlRender::unbind();
        return 1;
    }