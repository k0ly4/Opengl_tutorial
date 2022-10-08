#include "GraphicModule.h"

#include "EventModule.h"
#include "Scene.h"

/// <summary>
/// GraphicPipeline
/// </summary>
/// 
inline void display(const Texture2D& texture) {
    Debugger::display(texture);
}

void GraphicPipeline::initialize(RenderWindow& window) {

    frame.create(window.getSize(), TextureData(GL_RGBA16F, GL_RGBA, GL_NEAREST));
    gBuffer.create(window.getSize());
    ui.create(window.getSize(), TextureData(GL_RGBA, GL_RGBA, GL_NEAREST));

}

void GraphicPipeline::render(RenderWindow& window, Scene& scene, EventModule& event) {

    //Shadow ------------------
    ImageLoader::flipVerticallyOnLoad(0);
    scene.light.upShadowMap(scene, scene.camera);

    //gBuffer-------------------
    // in gbuffer
    window.setDefaultHintShader(glShader::gb_texture);
    gBuffer.render(window, scene);
    //out gbuffer
    GlRender::bind(frame);
    glClear(GL_COLOR_BUFFER_BIT);
    gBuffer.display(scene.light, scene.camera);

    //Forward render ----------------------
    window.setDefaultHintShader(glShader::light_texture);
    gBuffer.implementDepth(frame);
    scene.inForward(window);
    //UI----------------------
    GlRender::bind(ui);
    GlRender::setClearColor(0.f,0.f,0.f,0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    scene.inUI(ui);

    //Mix----------------------
    GlRender::unbind();
    Blend::Enable(true);
    Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
    Depth::Enable(false);
    glClear(GL_COLOR_BUFFER_BIT);

   // if (event.f.shadow_view) filter.displayRed(scene.light.getDirLight().getShadowMap(), event.f.shadow_level);
    //else 
    if (event.f.debugGbuffer)display(gBuffer.getTexture(0));
    else filter.drawExposure(frame.getTexture());
   
    if (event.f.shadow_view) 
        display(ui.getTexture());
   

    window.display();
}
//