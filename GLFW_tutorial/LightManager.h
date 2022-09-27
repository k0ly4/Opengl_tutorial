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
    inline void draw(View* view, const Shader& shader) {
        billboardLamp_.draw(view, shader);
    }

    void uniform(const Shader& shader, const Camera& camera);

private:

    float ambient_;
    std::list<PointLight>pLights_;
    DirectionLight dirLightGlobal_;

    Texture2D textureLamp_;
    Billboard billboardLamp_;

};
#endif