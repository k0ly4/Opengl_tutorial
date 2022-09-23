#include "Framebuffer.h"
#include "Exception.h"
    
/// <summary>
/// FrameBuffer
/// </summary>

    void FrameBuffer::implementDepth(unsigned int write_fbo) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, write_fbo);
        glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    void FrameBuffer::implementDepth(FrameBuffer& write_fbo) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, write_fbo.id);
        glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    void FrameBuffer::addTexture(GLint internalformat, GLint format, GLint filter) {
        t_data.push_back(TextureDataFormat(internalformat, format, filter));
    }

    void FrameBuffer::create(int width, int height)
    {
        try
        {
            size = glm::uvec2(width, height);
            std::vector<unsigned int> attachment(t_data.size());
            color.resize(t_data.size());

            GlRender::bind(*this);
            for (size_t i = 0; i < color.size(); i++) {

                color[i].gen();
                glTexture::bind2D(color[i]);

                glTexImage2D(GL_TEXTURE_2D, 0,
                    t_data[i].internal_format, width, height, 0, t_data[i].format,
                    t_data[i].internal_format == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, t_data[i].filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, t_data[i].filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                attachment[i] = GL_COLOR_ATTACHMENT0 + i;
                glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, color[i].getID(), 0);
            }
            glDrawBuffers(attachment.size(), attachment.data());

            glBindRenderbuffer(GL_RENDERBUFFER, depth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

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

    void RenderColor::addTexture(GLint internalformat, GLint format, GLint filter) {
        t_data.push_back(TextureDataFormat(internalformat, format, filter));
        color.push_back({});
        color.front().gen();
    }
   
    void RenderColor::create(int width, int height)
    {
        try
        {
        size = glm::ivec2(width, height);
        std::vector<unsigned int> attachment(t_data.size());

        GlRender::bind(*this);
        for (size_t i = 0; i < color.size(); i++) {
            glTexture::bind2D(color[i]);

            glTexImage2D(GL_TEXTURE_2D, 0, t_data[i].internal_format, width, height, 0, t_data[i].format,
                t_data[i].internal_format == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, t_data[i].filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, t_data[i].filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


            attachment[i] = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, color[i].getID(), 0);
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

    void RenderTexture::create(int width, int height, GLint format, GLint filterMin, GLint filterMax) {
        try
        {
            size = glm::uvec2(width, height);
            texture.filter(filterMin, filterMax);
            texture.wrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

            texture.create(width, height, format, format);
            glBindFramebuffer(GL_FRAMEBUFFER, id);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getID(), 0);

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