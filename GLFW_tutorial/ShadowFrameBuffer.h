#ifndef SHADOW_FRAMEBUFFER_H
#define SHADOW_FRAMEBUFFER_H
#include "Framebuffer.h"
class RenderClass {
public:
    virtual void drawScene_shadow(RenderTarget& target) = 0;
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
    std::vector<GeneralTexture> textures;
public:
    RenderCascadedDepth() {
        glGenFramebuffers(1, &id);
    }
    bool create(size_t cascadeLevels,const glm::ivec2& Size)
    {
        size = Size;
        textures.resize(cascadeLevels);
        for (size_t i = 0; i < cascadeLevels; i++) {
            textures[i].gen();
            glTexture::bind2D(textures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            //old
           /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float color_border[] = { 1.f,1.f,1.f,1.f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color_border);*/

            //new
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        GlRender::bind(*this);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[0].getID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            printf("(!)ERROR::FRAMEBUFFER::NON-COMPLETE\n");
            return 0;
        }     
        GlRender::unbind();
        return 1;
    }
    void bind_for_writing(size_t level){
        GlRender::bind(*this);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[level].getID(), 0);
    }
    void create(size_t cascadeLevels, int width, int height) {
        create(cascadeLevels,glm::ivec2(width, height));
    }
    Texture2D getTexture(size_t level) {
        return Texture2D(textures[level], size);
    }
};

#endif