#include "GraphicModule.h"

#include "EventModule.h"
#include "Scene.h"

/// <summary>
/// GraphicPipeline
/// </summary>
inline void display(const Texture2D& texture) {
    Debugger::display(texture);
}
void GraphicPipeline::initialize(RenderWindow& window) {
    frame.create(window.getSize(), TextureData(GL_RGBA16F, GL_RGBA, GL_NEAREST));
    gBuffer.create(window.getSize());
    ui.create(window.getSize(), TextureData(GL_RGBA, GL_RGBA, GL_NEAREST));
}

void GraphicPipeline::render(RenderWindow& window, Scene& scene, EventModule& event) {

    //shadow
    scene.light.upShadowMap(scene, scene.camera);
    //gbuffer
    gBuffer.render(window, scene);
    //in the frame
    GlRender::bind(frame);
    glClear(GL_COLOR_BUFFER_BIT);
    // GbufferRender
    gBuffer.display(scene.light, scene.camera);
    //Simple pipeline
    gBuffer.implementDepth(frame);

    window.draw(scene.light);

    GlRender::bind(ui);
    GlRender::setClearColor(0.f,0.f,0.f,0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    scene.inUI(ui);

    //exposure
    GlRender::unbind();
    Blend::Enable(true);
    Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
    GlRender::DepthTest(false);
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