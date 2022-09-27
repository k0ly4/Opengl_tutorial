#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "DirectionLight.h"
#include"PointLight.h"
#include"Billboard.h"

class LightSystem :public Drawable {
   
public:

    int shadow_demonstration = 0;

    LightSystem();

    PointLight& add(const PointLight& point_light, const View3D* view);

    inline DirectionLight& getDirLight() {
        return dirLightGlobal_;
    }

    inline void setAmbientFactor(float ambient) {
        ambient_ = ambient;
    }

    inline void draw(FrameBuffer& gBuffer, Camera& camera) {
        render(glShader::get(glShader::gb_light), gBuffer, camera);
    }

    inline void draw(View* view, const Shader& shader) {
        billboardLamp_.draw(view, shader);
    }

private:

    float ambient_;
    std::list<PointLight>pLights_;
    DirectionLight dirLightGlobal_;

    Texture2D textureLamp_;
    Billboard billboardLamp_;

    void render(const Shader& shader, FrameBuffer& gBuffer, Camera& camera);
};
#endif