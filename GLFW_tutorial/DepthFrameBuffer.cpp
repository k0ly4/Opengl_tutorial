#include "DepthFrameBuffer.h"

/// <summary>
/// RenderTextureDepth
/// </summary>

void RenderTextureDepth::create(const glm::ivec2& Size)
{
    size_ = Size;
    texture.reBuild();
    glTexture::bind2D(texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size_.x, size_.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float color_border[] = { 1.f,1.f,1.f,1.f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color_border);

    GlRender::bind(*this);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.get(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (GlRender::checkFramebufferStatus()) {
        log("(!)ERROR::FRAMEBUFFER::NON-COMPLETE\n");
    }
    GlRender::unbind();
}

/// <summary>
/// RenderCascadedDepth
/// </summary>

bool RenderCascadedDepth::create(size_t cascadeLevels, const glm::ivec2& size)
{
    if (map_.getSize() == size && map_.getCount()==cascadeLevels) {
        return 1;
    }

    map_.create(cascadeLevels, size, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT);
    map_.filter(TextureFilter::Nearest);
    map_.wrap(TextureWrap::ClampToBorder,TextureWrap::ClampToBorder, glm::vec4(1.f));

    GlRender::bind(*this);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, map_.getId().get(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (GlRender::checkFramebufferStatus()) {
        log("(!)ERROR::FRAMEBUFFER::RenderCascadedDepth::NON-COMPLETE\n");
        return 0;
    }

    GlRender::unbind();

    return 1;
}

/// <summary>
/// RenderCascadedDepth
/// </summary>

bool RenderDepthCubeMap::create(const glm::ivec2& size)
{
    if (map_.getSize() == size) {
        return 1;
    }
    proj_matrix_.set((float)size.x / (float)size.y, 90.f, 1.f, 25.f);
    map_.create(size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
    map_.filter(TextureFilter::Nearest);
    map_.wrap(TextureWrap::ClampToEdge);

    GlRender::bind(*this);
    map_.bindToFramebuffer();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (GlRender::checkFramebufferStatus()) {
        log("(!)ERROR::FRAMEBUFFER::RenderDepthCubeMap::NON-COMPLETE\n");
        return 0;
    }
    GlRender::unbind();
    return 1;
}

void RenderDepthCubeMap::uniform(const Shader& shader, const std::string& name, size_t unit) {
    map_.use(unit);
    shader.uniform(name + ".far_plane", proj_matrix_.getData().persp.far);
}

bool RenderDepthCubeMap::upLightPos(const glm::vec3& lightPos) {
    if (curLightPos == lightPos)
        return 0;

    curLightPos = lightPos;
    const glm::mat4& proj = proj_matrix_.get();
   
    transforms[0] = proj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    transforms[1] = proj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    transforms[2] = proj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    transforms[3] = proj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
    transforms[4] = proj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
    transforms[5] = proj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

    return 1;
}

void RenderDepthCubeMap::render(const View3D& view_player,const glm::vec3& lightPos, RenderScene& render)
{
    setView(view_player);
    upLightPos(lightPos);
    // ѕрив€зываем и очищаем текущий каскад
    GlRender::bind(*this);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, fbo.getTexture().getId().get(), 0);
    glClear(GL_DEPTH_BUFFER_BIT);

    const Shader& shader = glShader::get(glShader::cube_depth);
    shader.use();
    std::vector<glm::mat4> mats(6);
    for (size_t i = 0; i < 6; i++) {
        mats[i] = transforms[i];
    }
    shader.uniform("shadowMatrices", mats);
    shader.uniform("lightPos", curLightPos);
    shader.uniform("far_plane", proj_matrix_.getData().persp.far);
    render.inShadowMap(*this, glShader::cube_depth);
}

