#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#include "Texture.h"
#include "buffer.h"
#include<map>
#include<random>
#include"Render.h"

class FrameBuffer :public GeneralRender {
    
public:
    FrameBuffer() {
        glGenFramebuffers(1, &id);
        glGenRenderbuffers(1, &depth);
    }
    Texture2D getTexture(size_t index = 0) {
        return Texture2D(color[index], size);
    }
    void addTexture(GLint internalformat, GLint format, GLint filter);

    void create(int width, int height);

    void create(const glm::ivec2& size) {
        create(size.x, size.y);
    }

    void implementDepth(unsigned int write_fbo);

    void implementDepth(FrameBuffer& write_fbo);

    ~FrameBuffer() {
        glDeleteRenderbuffers(1, &depth);
        glDeleteFramebuffers(1, &id);
    }

private:
    unsigned int depth;

    std::vector<TextureDataFormat> t_data;
    std::vector<GeneralTexture> color;

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
};

class RenderColor :public GeneralRender {
 
public:

    RenderColor() {
        glGenFramebuffers(1, &id);
    }

    void addTexture(GLint internalformat, GLint format, GLint filter);

    void create(const glm::ivec2& size_window) {
        create(size_window.x, size_window.y);
    }

    void create(int width, int height);

    Texture2D getTexture(size_t index = 0) {
        return Texture2D(color[index],size);
    }

    ~RenderColor() {
        glDeleteFramebuffers(1, &id);
    }

private:
    std::vector<TextureDataFormat> t_data;
    std::vector<GeneralTexture> color;

    RenderColor(const RenderColor&) = delete;
    RenderColor& operator=(const RenderColor&) = delete;
};

class RenderTexture:public GeneralRender { 

public:

    RenderTexture() {
        glGenFramebuffers(1, &id);
    }

    void create(int width, int height, GLint format, GLint filterMin, GLint filterMax);

    inline Texture2D& getTexture() {
        return texture;
    }

    ~RenderTexture() {
        glDeleteFramebuffers(1, &id);
    }
private:
    Texture2D texture;

    RenderTexture(const RenderTexture&) = delete;
    RenderTexture& operator=(const RenderTexture&) = delete;
};
#endif