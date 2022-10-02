#include "CascadedShadow.h"

/// <summary>
/// CascadeViews
/// </summary>

void CascadeViews::computeEnds(float near, float far) {

    static const float factor_progression[] = {
        50.f,25.f,10.f,4.f,2.f
    };

    cascade_ends_[0] = near;
    for (size_t i = 1; i < cascade_ends_.size(); i++) {
        cascade_ends_[i] =far / factor_progression[i-1];
    }
}

void CascadeViews::compute(const View3D& view_player, const glm::vec3& lightDir) {
    if (curView_ == view_player) {
        return;
    }
    curView_.set(view_player);

    for (size_t i = 0; i < views_.size(); i++) {
        const auto proj = glm::perspective(
            glm::radians(curView_.persp.fov),
            curView_.persp.ratio,
            cascade_ends_[i],
            cascade_ends_[i + 1]);

        const std::vector<glm::vec4>corners(getFrustumCornersWorldSpace(proj, curView_.model));

        const glm::vec3 center(getCenter(corners));

        views_[i].setView(glm::lookAt(
            center + lightDir,
            center,
            GAME::WORLD_UP
        ));

        Box box(
            std::numeric_limits<float>::max(), //minX
            std::numeric_limits<float>::min(), //maxX
            std::numeric_limits<float>::max(), //minY
            std::numeric_limits<float>::min(), //maxY
            std::numeric_limits<float>::max(), //minZ
            std::numeric_limits<float>::min()  //maxZ
        );

        for (const auto& v : corners)
        {
            const auto trf = views_[i].getView().get() * v;

            box.minX = std::min(box.minX, trf.x);
            box.maxX = std::max(box.maxX, trf.x);
            box.minY = std::min(box.minY, trf.y);
            box.maxY = std::max(box.maxY, trf.y);
            box.minZ = std::min(box.minZ, trf.z);
            box.maxZ = std::max(box.maxZ, trf.z);

        }

        // Tune this parameter according to the scene
        if (box.minZ < 0)
        {
            box.minZ *= zMultFactor;
        }
        else
        {
            box.minZ /= zMultFactor;
        }

        if (box.maxZ < 0)
        {
            box.maxZ /= zMultFactor;
        }
        else
        {
        
           box.maxZ *= zMultFactor;
        }
      const Box box_debug(box.left, box.right, box.bottom, box.top,  -box.far, -box.near);
        views_[i].setProjection(box.getMatrix());
    }
}

void CascadeViews::uniform(const Shader& shader, const std::string& name) {
    for (size_t i = 0; i < views_.size(); i++) {

        shader.uniform(name + ".spaceMatrix[" + std::to_string(i) + "]", views_[i].getVP());
        shader.uniform(name + ".cascadePlaneDistances[" + std::to_string(i) + "]", cascade_ends_[i + 1]);

    }
}

std::vector<glm::vec4> CascadeViews::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
    const auto inv = glm::inverse(proj * view);

    std::vector<glm::vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const glm::vec4 pt =
                    inv * glm::vec4(
                        2.0f * x - 1.0f,
                        2.0f * y - 1.0f,
                        2.0f * z - 1.0f,
                        1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }

    return frustumCorners;
}
/// <summary>
/// CascadeShadow
/// </summary>
/// 
void CascadeShadow::render(const View3D & view_player, const glm::vec3& dirLight, RenderScene& render)
{
    views.compute(view_player, dirLight);
    fbo.setView(views[0]);
    // ѕрив€зываем и очищаем текущий каскад

    GlRender::bind(fbo);
   // glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, fbo.getTexture().getId().get(), 0);

    glEnable(GL_DEPTH_CLAMP);
    glClear(GL_DEPTH_BUFFER_BIT);
    glShader::get(glShader::cascades_shadow_depth).use();
    for (size_t i = 0; i < NUM_CASCADES; i++)
        glShader::get(glShader::cascades_shadow_depth).uniform("lightSpaceMatrices[" + std::to_string(i) + "]", views[i].getVP());
    render.inShadowMap(fbo, glShader::cascades_shadow_depth);
    glDisable(GL_DEPTH_CLAMP);
}
