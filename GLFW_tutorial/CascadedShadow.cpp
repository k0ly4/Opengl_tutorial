#include "CascadedShadow.h"

/// <summary>
/// CascadeViews
/// </summary>

void CascadeViews::computeEnds(float near, float far) {
    cascade_ends[0] = near;
    cascade_ends[1] = far / 50.0f;
    cascade_ends[2] = far / 25.f;
    cascade_ends[3] = far / 12.f;
    cascade_ends[4] = far / 7.f;
    cascade_ends[5] = far / 2.f;
}

void CascadeViews::compute(View3D& view_player, const glm::vec3& lightDir) {
    for (size_t i = 0; i < NUM_CASCADES; i++) {
        const auto proj = glm::perspective(
            glm::radians(view_player.getProjectionData().persp.fov), view_player.getProjectionData().persp.ratio, cascade_ends[i],
            cascade_ends[i + 1]);

        std::vector<glm::vec4>corners = getFrustumCornersWorldSpace(proj, view_player.getView().get());
        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const auto& v : corners)
        {
            center += glm::vec3(v);
        }
        center /= corners.size();

        view[i].setView(glm::lookAt(
            center + lightDir,
            center,
            GAME::WORLD_UP
        ));

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::min();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::min();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::min();
        for (const auto& v : corners)
        {
            const auto trf = view[i].getView().get() * v;
            minX = std::min(minX, trf.x);
            maxX = std::max(maxX, trf.x);
            minY = std::min(minY, trf.y);
            maxY = std::max(maxY, trf.y);
            minZ = std::min(minZ, trf.z);
            maxZ = std::max(maxZ, trf.z);
        }
        // Tune this parameter according to the scene
        constexpr float zMult = 10.0f;
        if (minZ < 0)
        {
            minZ *= zMult;
        }
        else
        {
            minZ /= zMult;
        }
        if (maxZ < 0)
        {
            maxZ /= zMult;
        }
        else
        {
            maxZ *= zMult;
        }
        view[i].setProjection(glm::ortho(minX, maxX, minY, maxY, minZ, maxZ));
    }
}

void CascadeViews::uniform(const Shader& shader, const std::string& name) {
    for (size_t i = 0; i < NUM_CASCADES; i++) {
        shader.uniform(name + ".spaceMatrix[" + std::to_string(i) + "]", view[i].getVP());
        shader.uniform(name + ".cascadePlaneDistances[" + std::to_string(i) + "]", cascade_ends[i + 1]);
    }
    // shader.uniform(name + ".cascadePlaneDistances[" + std::to_string(NUM_CASCADES) + "]", cascade_ends[NUM_CASCADES + 1]);
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
void CascadeShadow::render(View3D & view_player, RenderScene& render)
{
    views.compute(view_player, dirLight_);
    fbo.setView(views[0]);
    // ѕрив€зываем и очищаем текущий каскад
    fbo.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    glShader::get(glShader::cascades_shadow_depth).use();
    for (size_t i = 0; i < NUM_CASCADES; i++)
        glShader::get(glShader::cascades_shadow_depth).uniform("lightSpaceMatrices[" + std::to_string(i) + "]", views[i].getVP());
    render.inShadowMap(fbo, glShader::cascades_shadow_depth);
}
