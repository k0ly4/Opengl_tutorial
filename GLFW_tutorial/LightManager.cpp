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

    ImageLoader::flipVerticallyOnLoad(1);
    textureLamp_.loadFromFile("asset\\image\\lamp.png");

    id_obj = billboardLamp_.id_obj;
    billboardLamp_.setTexture(textureLamp_);
    billboardLamp_.setSize(glm::vec2(1.f));
}

void LightSystem::upShadowMap(RenderScene& scene,const Camera& camera) {
    dirLightGlobal_.upShadowMap(scene, camera);
    for (auto i = pLights_.begin(); i != pLights_.end(); i++) {
        i->upShadowMap(scene, camera);
    }
}

PointLight& LightSystem::add(const PointLight& pointLight, const View3D* view) {
    billboardLamp_.setPosition(pointLight.getPosition(), pLights_.size());
    billboardLamp_.setEye(view);
    pLights_.push_back(pointLight);
    return pLights_.front();
}

void LightSystem::uniform(const Shader& shader,const Camera& camera) {

    shader.uniform("viewPos", camera.getPosition());
    shader.uniform("ambientFactor", ambient_);
    shader.uniform("gWVP", camera.getVP());
    shader.uniform("debugMode", debugMode);
    dirLightGlobal_.uniform(SHADER_D_LIGHT, shader);
   
    size_t index = 0;
    for(auto i = pLights_.begin(); i!=pLights_.end(); i++, index++){
        i->uniform(SHADER_P_LIGHT+"[" + std::to_string(index) + "]", shader);
    }

    if (isShadow) {
        uniformShadow(shader, camera);
    }

}

void LightSystem::uniformShadow(const Shader& shader, const Camera& camera) { 
    dirLightGlobal_.uniformShadow(SHADER_D_LIGHT, shader, 3);
    size_t index = 0;
    for (auto i = pLights_.begin(); i != pLights_.end(); i++, index++) {
        i->uniformShadow(SHADER_P_LIGHT + "[" + std::to_string(index) + "]", shader, 4);
    }
}
