#include "GraphicModule.h"

#include "EventModule.h"
#include "Scene.h"

/// <summary>
/// GraphicPipeline
/// </summary>

void GraphicPipeline::initialize(RenderWindow& window) {
    frame.create(window.getSize(), TextureDataFormat(GL_RGBA16F, GL_RGBA, GL_NEAREST));
    gBuffer.create(window.getSize());
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

    //CullFace::Enable(false);
    window.draw(scene.light);

    //exposure
    GlRender::unbind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (event.f.shadow_view) filter.displayRed(scene.light.getDirLight().getShadowMap(), event.f.shadow_level);
    else if (event.f.debugGbuffer)Debugger::display(gBuffer.getTexture(0));
    else filter.drawExposure(frame.getTexture());

    window.display();
}
//