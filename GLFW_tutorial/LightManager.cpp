#include "LightManager.h"

LightSystem::LightSystem() {
    ambient_ = 0.1f;
    ImageLoader::flipVerticallyOnLoad(1);
    textureLamp_.loadFromFile("asset\\image\\lamp.png");

    id_obj = billboardLamp_.id_obj;
    billboardLamp_.setTexture(textureLamp_);
    billboardLamp_.setSize(glm::vec2(1.f));
}

PointLight& LightSystem::add(const PointLight& pointLight, const View3D* view) {
    billboardLamp_.setPosition(pointLight.getPosition(), pLights_.size());
    billboardLamp_.setEye(view);
    pLights_.push_back(pointLight);
    return pLights_.front();
}

void LightSystem::render(const Shader& shader, FrameBuffer& gBuffer, Camera& camera) {

    gBuffer.getTexture(0).use(0);
    gBuffer.getTexture(1).use(1);
    gBuffer.getTexture(2).use(2);

    shader.use();

    shader.uniform("viewPos", camera.getPosition());
    shader.uniform("ambientFactor", ambient_);
    shader.uniform("gWVP", camera.getVP());

    dirLightGlobal_.uniform("d_light", shader);

    size_t index = 0;
    for(auto i = pLights_.begin();i!=pLights_.end();i++,index++){
        i->uniform("p_light[" + std::to_string(index) + "]", shader);
    }

    sBuffer::quad.getVAO().draw();
}
