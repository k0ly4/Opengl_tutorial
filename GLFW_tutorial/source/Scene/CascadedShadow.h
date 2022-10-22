
#ifndef CASCADE_SHADOW_H
#define CASCADE_SHADOW_H
#include "Graphic/DepthFrameBuffer.h"

/// <summary>
/// ViewData
/// </summary>
struct cViewData {

    Perspective persp;
    glm::mat4 model;
    cViewData():
        model(1.f)
        {}
    void set(const View3D& view) {
        persp = view.getProjectionData().persp;
        model = view.getView().get();
    }
};
inline bool operator==(const cViewData& data, const View3D& view) {
    return (data.persp == view.getProjectionData().persp) && (data.model == view.getView().get());
}

/// <summary>
/// CascadeViews
/// </summary>
class CascadeViews {
   
public:    

    void create(size_t size_cascades){
        cascade_ends_.resize(size_cascades + 1);
        views_.resize(size_cascades);
        computeEnds(GAME::PROJECTION.near, GAME::PROJECTION.far);
    }

    void compute(const View3D& view_player, const glm::vec3& lightDir);

    void uniform(const Shader& shader, const std::string& name);

    ViewMatrix& operator [](size_t index) {
        return views_[index];
    }
    void setZMult(float factor) {
        zMultFactor = factor;
    }
private:
    
    cViewData curView_;
    std::vector<float> cascade_ends_;
    std::vector <ViewMatrix> views_;
    float zMultFactor = 10.0f;

    std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
    void computeEnds(float near, float far);
   
};

/// <summary>
/// CascadeShadow
/// </summary>
class CascadeShadow {

public:

    CascadeShadow():fbo(NUM_CASCADES, glm::ivec2(1024))
    { 
        views.create(NUM_CASCADES);
    }

    bool setSize(const glm::ivec2& render_size) {
        return fbo.create(NUM_CASCADES,render_size);
    }

    void setMult(float value) {
        views.setZMult(value);
    }

   const ArrayTexture2D& getTexture()const {
        return fbo.getTexture();
    }

    const View& getView(View3D& view_player, const glm::vec3& dirLight, size_t index_view) {
        views.compute(view_player, dirLight);
        return views[index_view];
    }

    void uniform(const Shader& shader, const std::string& name, size_t unit) {
        fbo.getTexture().use(unit);
        views.uniform(shader, name);
    }

    void uniform(const Shader& shader, size_t begin,size_t index, const std::string& name) {   
            shader.uniform(name + ".spaceMatrix", views[index].getVP());
            fbo.getTexture().use(begin);
    }  

    void render(const View3D& view_player, const glm::vec3& dirLight, RenderScene& render);

private:

    static const size_t NUM_CASCADES = 5;

    RenderCascadedDepth fbo;
    CascadeViews views;
};

#endif
