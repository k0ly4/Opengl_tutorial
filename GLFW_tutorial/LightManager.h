#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H
#include "DIrectionLight.h"
#include"PointLight.h"
class LightSystem :public Drawable {
    void render(const Shader& shader, FrameBuffer& gBuffer,Camera& camera) {
        shader.use();
        gBuffer.getTexture(0).use(0);
        gBuffer.getTexture(1).use(1);
        gBuffer.getTexture(2).use(2);
        shader.uniform("viewPos", camera.getPosition());
        shader.uniform("ambientFactor", ambient);
        shader.uniform("gWVP", camera.getVP());
        d_light.uniform("d_light", shader);
        for (size_t i = 0; i < p_light.size(); i++)
            p_light[i].uniform("p_light[" + std::to_string(i) + "]", shader);
        sBuffer::quad.getVAO().draw();
    }
    float ambient;
    std::vector<P_Light> p_light;
    D_Light d_light;
    Texture2D texture_lamp;
    Billboard billboard_lamp;
public:
    int shadow_demonstration = 0;
    LightSystem() {
        id_obj = billboard_lamp.id_obj;
        texture_lamp.loadFromFile("asset\\image\\lamp.png", 1, 1, 1);
        billboard_lamp.setTexture(texture_lamp);
        billboard_lamp.setSize(glm::vec2(1.f));
    }
    void add(const P_Light& point_light,const View3D* view) {
        p_light.push_back(point_light);
        billboard_lamp.setPosition(point_light.position, p_light.size() - 1);
        billboard_lamp.setEye(view);
    }
    D_Light& global() {
        return d_light;
    }
    void setAmbientFactor(float ambient) {
        this->ambient = ambient;
    }
    void draw(FrameBuffer& gBuffer, Camera& camera) {
        render(glShader::get(glShader::gb_light), gBuffer, camera);
    }
    void draw(View* view, const Shader& shader) {
        billboard_lamp.draw(view, shader);
    }
};
#endif