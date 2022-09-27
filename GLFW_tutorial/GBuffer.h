#ifndef GBUFFER_H
#define GBUFFER_H

#include "Framebuffer.h"

class GBuffer:public FrameBuffer {

public:

    void create(const glm::ivec2 size) {
        TextureDataFormat dataFormats[3] = { 
            TextureDataFormat (GL_RGBA16F, GL_RGBA, GL_NEAREST),    //Position
            TextureDataFormat (GL_RGBA16F, GL_RGBA, GL_NEAREST),    //Normal
            TextureDataFormat (GL_RGBA, GL_RGBA, GL_NEAREST)        //Diffuse + Specular
        };      
        FrameBuffer::create(size, 3, dataFormats);
    }

    void bindPositionMap() {
        glTexture::bind2D(textures_[0]);
    }

    void bindNormalMap() {
        glTexture::bind2D(textures_[1]);
    }

    void bindDiffuseMap() {
        glTexture::bind2D(textures_[2]);
    }

private:
};
#endif