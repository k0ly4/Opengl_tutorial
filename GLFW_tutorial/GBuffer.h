#ifndef GBUFFER_H
#define GBUFFER_H

#include "LightManager.h"


/// <summary>
/// GBuffer
/// </summary>
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

    inline void usePositionMap(size_t unit) {
        glTexture::active(GL_TEXTURE0 + unit);
        glTexture::bind2D(textures_[0]);
    }

    inline void useNormalMap(size_t unit) {
        glTexture::active(GL_TEXTURE0 + unit);
        glTexture::bind2D(textures_[1]);
    }
    inline void useDiffuseMap(size_t unit) {
        glTexture::active(GL_TEXTURE0 + unit);
        glTexture::bind2D(textures_[2]);
    }
    inline void render(void (*renderScene)()) {

        GlRender::setClearColor(Color(0.f));
        GlRender::bind(*this);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderScene();

    }
    void render(RenderWindow& window, RenderScene& scene) {

        GlRender::setClearColor(Color(0.f));
        GlRender::bind(*this);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.inGBuffer(window);

    }
    void display(LightSystem& lights,const Camera& camera) {
        const Shader& shader = glShader::get(glShader::gb_light);

        shader.use();
        usePositionMap(0);
        useNormalMap(1);
        useDiffuseMap(2);
        lights.uniform(shader, camera);
        sBuffer::quad.getVAO().draw();
    }
   
private:

};
#endif