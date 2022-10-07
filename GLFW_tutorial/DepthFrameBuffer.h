#ifndef DEPTH_FRAMEBUFFER_H
#define DEPTH_FRAMEBUFFER_H
#include "Framebuffer.h"


/// <summary>
/// RenderTextureDepth
/// </summary>
class RenderTextureDepth :public GeneralRender, public RenderTarget,public Sizeable {
    
public:

    RenderTextureDepth() {
        glGenFramebuffers(1, &id_);
    }

    void create(const glm::ivec2& Size);

    void create(int width, int height) {
        create(glm::ivec2(width, height));
    }

    Texture2D getTexture() {
        return Texture2D(texture, size_);
    }

    const glm::ivec2& getSize()const {
        return size_;
    }
    ~RenderTextureDepth() {
        glDeleteFramebuffers(1, &id_);
    }
private:

    TexturePointer texture;

    RenderTextureDepth(const RenderTextureDepth&) = delete;
    RenderTextureDepth& operator=(const RenderTextureDepth&) = delete;
};

/// <summary>
/// RenderCascadedDepth
/// </summary>
class  RenderCascadedDepth :public GeneralRender, public RenderTarget {
    
public:

    RenderCascadedDepth() {
        glGenFramebuffers(1, &id_);
    }
    RenderCascadedDepth( size_t numCascades, const glm::ivec2& size) {
        glGenFramebuffers(1, &id_);
        create(numCascades, size);
    }
    bool create(size_t cascadeLevels, const glm::ivec2& Size);
    bool create(size_t cascadeLevels, int width, int height) {
       return create(cascadeLevels, glm::ivec2(width, height));
    }

    const ArrayTexture2D& getTexture()const {
        return map_;
    }

    const glm::ivec2& getSize()const {
        return map_.getSize();
    }
    ~RenderCascadedDepth() {
        glDeleteFramebuffers(1, &id_);
    }
private:
    ArrayTexture2D map_;

    RenderCascadedDepth(const RenderCascadedDepth&) = delete;
    RenderCascadedDepth& operator=(const RenderCascadedDepth&) = delete;
};

/// <summary>
/// RenderDepthMap
/// </summary>
class  RenderDepthCubeMap :public GeneralRender, public RenderTarget {

public:

    RenderDepthCubeMap():
        lastLightPos(0.f) {
        glGenFramebuffers(1, &id_);
    }

    RenderDepthCubeMap(const glm::ivec2& size,float far_plane):
        lastLightPos(0.f) {
        glGenFramebuffers(1, &id_);
        create(size, far_plane);
    }

    bool create(const glm::ivec2& size, float far_plane);
    bool create(int width, int height, float far_plane) {
        return create(glm::ivec2(width, height),far_plane);
    }

    const TextureCubeMap& getTexture()const {
        return map_;
    }

    void setFarPlane(float far_plane) {
        if (proj_matrix_.getData().persp.far == far_plane)return;
        constexpr float near_plane = 1.f;
        proj_matrix_.set(proj_matrix_.getData().persp.ratio, 90.f, near_plane, far_plane);
        needUpTransforms = 1;
    }

    const glm::ivec2& getSize()const {
        return map_.getSize();
    }

    void uniform(const Shader& shader, const std::string& name, size_t unit);

    void render(const View3D& view_player, const glm::vec3& lightPos, RenderScene& render);

    ~RenderDepthCubeMap() {
        glDeleteFramebuffers(1, &id_);
    }
private:

    bool upLightPos(const glm::vec3& lightPos);
    bool needUpTransforms = 1;

    glm::mat4 transforms[6];
    glm::vec3 lastLightPos;
    TextureCubeMap map_;
    ProjectionMatrix proj_matrix_;

    RenderDepthCubeMap(const RenderDepthCubeMap&) = delete;
    RenderDepthCubeMap& operator=(const RenderDepthCubeMap&) = delete;
};

#endif