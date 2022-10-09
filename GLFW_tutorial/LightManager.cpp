#include "LightManager.h"

static const std::string SHADER_P_LIGHT = "p_light";
static const std::string SHADER_D_LIGHT = "d_light";


/// <summary>
/// LightSystem
/// </summary>
/// 
LightSystem::LightSystem() {
    isShadow = 1;
    ambient_ = 0.1f;

    lightPointSphere_.setRadius(1.f);
    lightPointSphere_.setCountSector(6);
    lightPointSphere_.setCountStack(6);

    ImageLoader::flipVerticallyOnLoad(1);
    textureLamp_.getPath("asset\\image\\lamp.png");

    shaderHint = billboardLamp_.getShaderHint();
    billboardLamp_.setTexture(textureLamp_);
    billboardLamp_.setSize(glm::vec2(1.f));
    pLights_.reserve(10);
}

void LightSystem::upShadowMap(RenderScene& scene,const Camera& camera) {
    dirLightGlobal_.upShadowMap(scene, camera);
    for (auto i = pLights_.begin(); i != pLights_.end(); i++) {
        i->upShadowMap(scene, camera);
    }
}

size_t LightSystem::add(const PointLight& pointLight, const View3D* view) {
    billboardLamp_.setPosition(pointLight.getPosition(), pLights_.size());
    billboardLamp_.setEye(view);
    pLights_.push_back(pointLight);
    return pLights_.size()-1;
}

void LightSystem::uniform(const Shader& shader,const Camera& camera,size_t unit_free) {
    shader.uniform("ambientFactor", ambient_);
    shader.uniform("viewPos", camera.getPosition());
    dirLightGlobal_.uniform(SHADER_D_LIGHT, shader);
  
    for (size_t i = 0; i < pLights_.size(); i++) {
        pLights_[i].uniform(SHADER_P_LIGHT + "[" + std::to_string(i) + "]", shader);
    }

    if (isShadow) {
        uniformShadow(shader, camera, unit_free);
    }

}

void LightSystem::renderDirLights(const Shader& shader, const Camera& camera, size_t unitMap) {
    if (dirLightGlobal_.isActive() == 0) 
        return;
    shader.uniform("ambientFactor", ambient_);

    dirLightGlobal_.uniform(SHADER_D_LIGHT, shader);
    if (isShadow) dirLightGlobal_.uniformShadow(SHADER_D_LIGHT, shader, unitMap);

    sBuffer::quad.getVAO().draw();
}

void LightSystem::renderPointLights(const Shader& shader, const Camera& camera, size_t unitMap) {

    shader.uniform("ambientFactor", ambient_);

    for (size_t i = 0; i < pLights_.size(); i++) {

        pLights_[i].uniform(SHADER_P_LIGHT + "[" + std::to_string(i) + "]", shader);
        if(isShadow) pLights_[i].uniformShadow(SHADER_P_LIGHT + "[" + std::to_string(i) + "]", shader, unitMap + i);
        
        lightPointSphere_.setScale(glm::vec3(pLights_[i].getRadius()));
        lightPointSphere_.setPosition(pLights_[i].getPosition());
        lightPointSphere_.draw(&camera, shader);;

    }

}

void LightSystem::uniformShadow(const Shader& shader, const Camera& camera,size_t unitMap) { 
    dirLightGlobal_.uniformShadow(SHADER_D_LIGHT, shader, unitMap);
    unitMap++;
    for (size_t i = 0; i < pLights_.size(); i++) {
        pLights_[i].uniformShadow(SHADER_P_LIGHT + "[" + std::to_string(i) + "]", shader, unitMap + i);
    }
}
