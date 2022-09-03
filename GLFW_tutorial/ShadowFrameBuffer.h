#ifndef SHADOW_FRAMEBUFFER_H
#define SHADOW_FRAMEBUFFER_H
#include "Framebuffer.h"
class RenderClass {
public:
    virtual void drawScene_shadow(RenderTarget& target, glShader::Object shader) = 0;
};
class  RenderTextureDepth :public GeneralRender, public RenderTarget {
    GeneralTexture texture;
public:
    RenderTextureDepth() {
        glGenFramebuffers(1, &id);
    }
    void create(const glm::ivec2& Size)
    {
        size = Size;
        texture.gen();
        glTexture::bind2D(texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float color_border[] = { 1.f,1.f,1.f,1.f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color_border);

        GlRender::bind(*this);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.getID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "(!)ERROR::FRAMEBUFFER::NON-COMPLETE" << std::endl;
        }
        GlRender::unbind();
    }
    void create(int width, int height) {
        create(glm::ivec2(width, height));
    }
    Texture2D getTexture() {
        return Texture2D(texture, size);
    }
};
class  RenderCascadedDepth :public GeneralRender, public RenderTarget {
    GeneralTexture texture;
public:
    RenderCascadedDepth() {
        glGenFramebuffers(1, &id);
    }
    bool create(int cascadeLevels,const glm::ivec2& Size)
    {
        size = Size;
       
            texture.gen();
            glTexture::bind2DArray(texture.getID());
        
            glTexImage3D(
                GL_TEXTURE_2D_ARRAY,
                0,
                GL_DEPTH_COMPONENT32F,
                Size.x,
                Size.y,
                cascadeLevels,
                0,
                GL_DEPTH_COMPONENT,
                GL_FLOAT,
                nullptr);

            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

            constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);
      
        GlRender::bind(*this);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture.getID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            printf("(!)ERROR::FRAMEBUFFER::NON-COMPLETE\n");
            return 0;
        }     
        GlRender::unbind();
        return 1;
    }
    void bind() {
        GlRender::bind(*this);
    }
    void create(size_t cascadeLevels, int width, int height) {
        create(cascadeLevels,glm::ivec2(width, height));
    }
    g_ArrayTexture2D getTexture() {
        return g_ArrayTexture2D(texture);
    }
};

#endif