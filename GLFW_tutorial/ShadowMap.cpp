#include "ShadowMap.h"

void ShadowMap::uniform(const Shader& shader, size_t unit, const std::string& name) {
    map.getTexture().use(unit);
    shader.uniform(name + ".spaceMatrix", view.getVP());
}

void ShadowMap::render(void (*render_scene) (RenderTarget& target))
{
    // ѕрив€зываем и очищаем текущий каскад
    GlRender::bind(map);
    glClear(GL_DEPTH_BUFFER_BIT);
    render_scene(map);
}

void ShadowMap::render(RenderScene* render)
{
    GlRender::bind(map);
    glClear(GL_DEPTH_BUFFER_BIT);
    render->inShadowMap(map, glShader::shadow_depth);
}
