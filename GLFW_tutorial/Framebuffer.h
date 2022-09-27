#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#include "Texture.h"
#include "buffer.h"
#include<map>
#include<random>
#include"Render.h"

/// <summary>
/// RenderClass
/// </summary>
class RenderClass {
public:
    virtual void drawScene_shadow(RenderTarget& target, glShader::Object shader) = 0;
};

/// <summary>
/// FrameBuffer
/// </summary>
class FrameBuffer :public GeneralRender {
    
public:
    FrameBuffer() {
        glGenFramebuffers(1, &id_);
        glGenRenderbuffers(1, &rendereBuffer_);
    }

    Texture2D getTexture(size_t index = 0) {
        return Texture2D(textures_[index], size_);
    }
    /// <summary>
    /// create
    /// </summary>
    /// <param name="width"></param>
    /// <param name="height"></param
    void create(const glm::ivec2& render_size, size_t sizeTextures, const TextureDataFormat* setupTextures);

    void create(int width, int height, size_t sizeTextures, const TextureDataFormat* setupTexture) {
        create(glm::ivec2(width, height), sizeTextures, setupTexture);
    }

    void create(const glm::ivec2& render_size, const std::vector<TextureDataFormat>& textures) {
        create(render_size, textures.size(), textures.data());
    }

    void create(const glm::ivec2& render_size,const TextureDataFormat& texture_format) {
        create(render_size, 1, &texture_format);
    }

    void setSize(const glm::ivec2& render_size);

    void implementDepth(unsigned int write_fbo);

    inline void implementDepth(FrameBuffer& write_fbo) {
        implementDepth(write_fbo.id_);
    }

    ~FrameBuffer() {
        glDeleteRenderbuffers(1, &rendereBuffer_);
        glDeleteFramebuffers(1, &id_);
    }

protected:
    unsigned int rendereBuffer_;
    std::vector<TexturePointer> textures_;
    std::vector<TextureDataFormat> texturesFormat_;
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
};

/// <summary>
/// RenderColor
/// </summary>
class RenderColor :public GeneralRender {
 
public:

    RenderColor() {
        glGenFramebuffers(1, &id_);
    }

    void create(int width, int height, size_t sizeTextures, const TextureDataFormat* setupTexture) {
        create(glm::ivec2(width, height), sizeTextures, setupTexture);
}

    void create(const glm::ivec2& render_size, const std::vector<TextureDataFormat>& textures){
        create(render_size, textures.size(), textures.data());
    }

    void create(const glm::ivec2& render_size, size_t sizeTextures, const TextureDataFormat* setupTextures);

    Texture2D getTexture(size_t index = 0) {
        return Texture2D(textures_[index],size_);
    }

    ~RenderColor() {
        glDeleteFramebuffers(1, &id_);
    }

private:
    std::vector<TexturePointer> textures_;

    RenderColor(const RenderColor&) = delete;
    RenderColor& operator=(const RenderColor&) = delete;
};

/// <summary>
/// RenderTexture
/// </summary>
class RenderTexture:public GeneralRender { 

public:

    RenderTexture() {
        glGenFramebuffers(1, &id_);
    }

    void create(int width, int height, GLint format, GLint filterMin, GLint filterMax);

    inline Texture2D& getTexture() {
        return texture;
    }

    ~RenderTexture() {
        glDeleteFramebuffers(1, &id_);
    }
private:
    Texture2D texture;

    RenderTexture(const RenderTexture&) = delete;
    RenderTexture& operator=(const RenderTexture&) = delete;
};
#endif