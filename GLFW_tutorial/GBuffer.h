#ifndef GBUFFER_H
#define GBUFFER_H

#include "LightManager.h"


/// <summary>
/// GBuffer
/// </summary>
class GBuffer:public FrameBuffer {

public:

    void create(const glm::ivec2 size) {
       const TextureData dataFormats[3] = { 
            TextureData(GL_RGBA16F, GL_RGBA, GL_NEAREST),    //Position
            TextureData(GL_RGBA16F, GL_RGBA, GL_NEAREST),    //Normal
            TextureData(GL_RGBA, GL_RGBA, GL_NEAREST)        //Diffuse + Specular
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

    void render(RenderWindow& window, RenderScene& scene) {

        GlRender::setClearColor(Color(0.f));
        GlRender::bind(*this);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.inGBuffer(window);

    }

    void setDebugMode(bool enable) {
        debugMode = enable;
    }

    void display(LightSystem& lights,const Camera& camera) {

        Blend::Enable(true);
        Blend::Equation(Blend::Add);
        Blend::Func(Blend::One, Blend::One);

        glClear(GL_COLOR_BUFFER_BIT);
        usePositionMap(0);
        useNormalMap(1);
        useDiffuseMap(2);
        CullFace::Enable(true);
        CullFace::Mode(CullFace::ModeEnum::Front);
        displayPointLight(lights,glShader::get(glShader::gb_render_point_lights), camera);
        //displayDirLight(lights,glShader::get(glShader::gb_render_dir_lights), camera);
     
        //lights.uniform(shader, camera,3);
        //sBuffer::quad.getVAO().draw();
    }
   
private:

    void displayPointLight(LightSystem& lights, const Shader& shader, const Camera& camera) {
        
        shader.use();
        shader.uniform("viewPos", camera.getPosition());
        shader.uniform("debugMode", debugMode);
        shader.uniform("gScreenSize", glm::vec2(size_.x,size_.y));
        lights.renderPointLights(shader, camera,3);

    }
    void displayDirLight(LightSystem& lights, const Shader& shader,const Camera& camera) {
      
        shader.use();
        shader.uniform("viewPos", camera.getPosition());
        shader.uniform("debugMode", debugMode);
        shader.uniform("gWVP", camera.getVP());
        lights.renderDirLights(shader, camera, 3);

    }

    int debugMode = 0;
};
#endif