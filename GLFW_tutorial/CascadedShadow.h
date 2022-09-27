
#ifndef CASCADE_SHADOW_H
#define CASCADE_SHADOW_H
#include"DepthFrameBuffer.h"

#define NUM_CASCADES 5

class CascadeViews {
   
public:    

    CascadeViews() {
        cascade_ends.resize(NUM_CASCADES+1);
        computeEnds(GAME::PROJECTION.near,GAME::PROJECTION.far);
    }

    void compute(View3D& view_player, const glm::vec3& lightDir);

    void uniform(const Shader& shader, const std::string& name);

    ViewMatrix& operator [](size_t index) {
        return view[index];
    }

private:

    std::vector<float> cascade_ends;
    ViewMatrix view[NUM_CASCADES];

    std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
   
    void computeEnds(float near, float far);
   
};
class CascadeShadow {

public:

    CascadeShadow():dirLight_(0.f) 
    {
        fbo.create(NUM_CASCADES, GAME::RENDER_SIZE);    
    }

    ArrayTexture2D getTexture() {
        return fbo.getTexture();
    }
 
    void setDirectionLight(const glm::vec3& dirLight) {
        dirLight_ = dirLight;
    }

    const View& getView(View3D& view_player, size_t index_view) {
        views.compute(view_player, dirLight_);
        return views[index_view];
    }

    void uniform(const Shader& shader, const std::string& name, size_t begin) {
        fbo.getTexture().use(begin);
        views.uniform(shader, name);
    }

    void uniform(const Shader& shader, size_t begin,size_t index, const std::string& name) {   
            shader.uniform(name + ".spaceMatrix", views[index].getVP());
            fbo.getTexture().use(begin);
    }  

    void render(View3D& view_player, RenderClass* render);

private:

    RenderCascadedDepth fbo;
    CascadeViews views;
    glm::vec3 dirLight_;

};

#endif
