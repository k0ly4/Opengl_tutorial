#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include    "DirectionLight.h"
#include    "PointLight.h"
#include    "Billboard.h"
#include    "Sphere.h"

static const std::string SHADER_P_LIGHT = "p_light";
static const std::string SHADER_D_LIGHT = "d_light";

/// <summary>
/// ManagerPointLight
/// </summary>
class ManagerPointLight: public Drawable {
public:

    ManagerPointLight() {
        ImageLoader::flipVerticallyOnLoad(1);
        textureLamp_.getPath("asset\\image\\lamp.png");

        shaderHint = billboardLamp_.getShaderHint();
        billboardLamp_.setTexture(textureLamp_);
        billboardLamp_.setSize(glm::vec2(1.f));
        pLights_.reserve(10);
    }

    void uniformPoint(size_t index, const Shader& shader, int unit_map_shadow=-1) {

        pLights_[index].uniform(SHADER_P_LIGHT + "[" + std::to_string(index) + "]", shader);

        if (unit_map_shadow >-1)
            pLights_[index].uniformShadow(SHADER_P_LIGHT + "[" + std::to_string(index) + "]", shader, unit_map_shadow);
    }

    size_t push_back(const PointLight& pointLight, const View3D* view) {
        billboardLamp_.setPosition(pointLight.getPosition(), pLights_.size());
        billboardLamp_.setEye(view);
        pLights_.push_back(pointLight);
        return pLights_.size() - 1;
    }

    void upShadows(RenderScene& scene, const Camera& camera) {
            for (auto i = pLights_.begin(); i != pLights_.end(); i++) {
                i->upShadowMap(scene, camera);
            }
    }

    PointLight& operator[](size_t index) {
        return pLights_[index];
    }

    size_t size() {
        return pLights_.size();
    }

    inline void draw(const View* view, const Shader& shader) {
        size_t index = 0;
        for (auto l = pLights_.begin(); l != pLights_.end(); l++, index++) {
            billboardLamp_.setPosition(l->getPosition(), index);
        }
        billboardLamp_.draw(view, shader);
    }

private:

    Texture2D textureLamp_;
    Billboard billboardLamp_;
    std::vector<PointLight>pLights_;
};

/// <summary>
/// LightSystem
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
class LightSystem {
   
public:

    int shadow_demonstration = 0;

    LightSystem() {
        isShadow = 1;
        ambient_ = 0.1f;
    }

    void upShadowMap(RenderScene& scene, const Camera& camera) {
        lightTest.upShadowMap(scene, camera);
        global_.upShadowMap(scene, camera);
        pLight_.upShadows(scene, camera);
      
    }

    void setShadowMode(bool enable) {
        isShadow = enable;
    }
    
    inline DirectionLight& getDirs() {
        return global_;
    }

    inline ManagerPointLight& getPoints() {
        return pLight_;
    }

    inline void setAmbientFactor(float ambient) {
        ambient_ = ambient;
    } 

    void renderPointInGBuffer(const Shader& shader, const Camera& camera, size_t unitMap) {

        shader.uniform("ambientFactor", 0.f);
        for (size_t i = 0; i < pLight_.size(); i++) {
            if (pLight_[i].isActive() == 0) continue;
            pLight_.uniformPoint(i, shader, isShadow ? unitMap: -1);
            sBuffer::quad.getVAO().draw();
        }

    }
    void renderDirLightsInGBuffer(const Shader& shader, const Camera& camera, size_t unitMap) {
        if (global_.isActive() == 0) {
            return;
        }
        //Uniform
        globalUniform(shader);
        global_.uniform(SHADER_D_LIGHT, shader);
        if (isShadow) global_.uniformShadow(SHADER_D_LIGHT, shader, unitMap);
        //Draw
        sBuffer::quad.getVAO().draw();
    }

    void renderSimpleLight(const Shader& shader, const Camera& camera, size_t unitMap) {
        //Uniform
        globalUniform(shader);
       // shader.uniform("ambientFactor", 0.f);
        lightTest.uniform(SHADER_D_LIGHT, shader);
        if (isShadow)lightTest.uniformShadow(SHADER_D_LIGHT, shader, unitMap);
        //Draw
        sBuffer::quad.getVAO().draw();
    }

    void draw(RenderTarget& target,const Camera& camera, Drawable& object) {
       
        Depth::Mask(false);
        drawPointLight(target, useShader(glShader::texture_pointLight, camera), camera, object, 1);
        
        Depth::Mask(true);
        if (pLight_[0].isActive() == 1) {
            Blend::Equation(Blend::Add);
            Blend::Func(Blend::One, Blend::One);
        }
        drawGlobal(target, useShader(glShader::texture_dirLight, camera), camera, object, 1);
       
    }

     SimpleDirLight lightTest;

private:

    bool isShadow;
    float ambient_;

    ManagerPointLight pLight_;
    DirectionLight global_;
    

    void drawGlobal(RenderTarget& target,const Shader& shader, const Camera& camera, Drawable& object, size_t unitMap) {
        //Uniform
        globalUniform(shader);
        if (global_.isActive()) {
            global_.uniform(SHADER_D_LIGHT, shader);
            if (isShadow) global_.uniformShadow(SHADER_D_LIGHT, shader, unitMap);
        }
        else {
            
        }
        //Render
        target.draw(object, shader);
    }

    void drawPointLight(RenderTarget& target, const Shader& shader, const Camera& camera, Drawable& object, size_t unitMap) {
        shader.uniform("ambientFactor", 0.f);
        globalUniform(shader);
        for (size_t i = 0; i < pLight_.size(); i++) {
            if (pLight_[i].isActive() == 0) continue;
            //Uniform
            pLight_.uniformPoint(i, shader, isShadow ? unitMap : -1);
            //Render
            target.draw(object, shader);
        }
    }
    const Shader& useShader(glShader::Object id,const Camera& camera) {
        const Shader& shader = glShader::get(id);
        shader.use();
        shader.uniform("viewPos", camera.getPosition());
        return shader;
    }
    void globalUniform(const Shader& shader){
        shader.uniform("ambientFactor", ambient_);
    }
};
#endif