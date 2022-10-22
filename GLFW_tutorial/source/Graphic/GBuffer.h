#ifndef GBUFFER_H
#define GBUFFER_H

#include "Scene/LightManager.h"


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
       
        Depth::Enable(false);
        glClear(GL_COLOR_BUFFER_BIT);
        usePositionMap(0);
        useNormalMap(1);
        useDiffuseMap(2);
       
        Blend::Equation(Blend::Add);
        Blend::Func(Blend::One, Blend::One); 
        renderDirLights(lights, glShader::get(glShader::gb_render_dir_lights), camera);
       //renderDirLightsTest(lights, glShader::get(glShader::gb_render_dir_lights_test), camera);
        renderPointLights(  lights, glShader::get(glShader::gb_render_point_lights),    camera);  
      
    }
   
private:
    void renderPointLights(LightSystem& lights, const Shader& shader, const Camera& camera) {
        shader.use();
        globalUniform(shader, camera);
        lights.renderPointInGBuffer(shader, camera,3);

    }
    void renderDirLights(LightSystem& lights, const Shader& shader,const Camera& camera) {
        shader.use();
        globalUniform(shader,camera);
        shader.uniform("gWVP", camera.getVP());
        lights.renderDirLightsInGBuffer(shader, camera, 3);
    }
    void renderDirLightsTest(LightSystem& lights, const Shader& shader, const Camera& camera) {
        shader.use();
        globalUniform(shader, camera);
        lights.renderSimpleLight(shader, camera, 3);
    }

    void globalUniform(const Shader& shader, const Camera& camera) {
        shader.uniform("viewPos", camera.getPosition());
        shader.uniform("debugMode", debugMode);
    }

    int debugMode = 0;
};
#endif