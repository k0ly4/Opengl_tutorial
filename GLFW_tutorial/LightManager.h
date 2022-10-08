#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "DirectionLight.h"
#include"PointLight.h"
#include"Billboard.h"

class LightSystem :public Drawable {
   
public:

    int shadow_demonstration = 0;

    LightSystem();

    size_t add(const PointLight& point_light, const View3D* view);

    void upShadowMap(RenderScene& scene, const Camera& camera);

    void setShadowMode(bool enable) {
        isShadow = enable;
    }
    
    inline DirectionLight& getDirLight() {
        return dirLightGlobal_;
    }

    inline void setAmbientFactor(float ambient) {
        ambient_ = ambient;
    }
    inline void draw(const View* view, const Shader& shader) {
        size_t index = 0;
        for (auto l = pLights_.begin(); l != pLights_.end(); l++, index++) {
            billboardLamp_.setPosition(l->getPosition(), index);
        }
        billboardLamp_.draw(view, shader);
    }
    PointLight& getPoint(size_t id) {
        return pLights_[id];
    }
    void uniform(const Shader& shader, const Camera& camera, size_t unit_free);
    void draw(RenderTarget& target,const Camera& camera, Drawable& object) {
        const Shader& shader = glShader::get(target.getHintShader(object));
        shader.use();
        uniform(shader, camera, 1);
        target.draw(object, shader);
    }
private:

    bool isShadow;
    float ambient_;
    std::vector<PointLight>pLights_;
    DirectionLight dirLightGlobal_;

    Texture2D textureLamp_;
    Billboard billboardLamp_;

    void uniformShadow(const Shader& shader, const Camera& camera, size_t unitMap);

};
#endif