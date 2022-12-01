#include "GraphicModule.h"

#include "Input/EventModule.h"
#include "Scene/Scene.h"

/// <summary>
/// GraphicPipeline
/// </summary>
/// 
inline void display(const Texture2D& texture) {
    Debugger::display(texture);
}

void GraphicPipeline::initialize(RenderWindow& window) {

    frame.create(window.getSize(), TextureData(GL_RGBA16F, GL_RGBA, GL_NEAREST));
  /*  gBuffer.create(window.getSize());*/
    ui.create(window.getSize(), TextureData(GL_RGBA, GL_RGBA, GL_NEAREST));
}

void GraphicPipeline::render(RenderWindow& window, Scene& scene, EventModule& event) {
    ///--------------------------------------------
    //Shadow ------------------
    if (false) {
    //    scene.light.upShadowMap(scene, scene.camera); //нет необходимости
    ////gBuffer-------------------
    //// in gbuffer //нет необходимости
    //    window.setDefaultHintShader(glShader::gb_texture);
    //    gBuffer.render(window, scene); 
    //    //out gbuffer
    //    GlRender::bind(frame);
    //    glClear(GL_COLOR_BUFFER_BIT);
    //    gBuffer.display(scene.light, scene.camera);
    //    gBuffer.implementDepth(frame);
    }
    ////////////////---------------------------------
    else {
    GlRender::bind(frame);
    GlRender::setClearColor(0.7f, 0.7f, 1.f,1.f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    }
    //Forward render ----------------------
    window.setDefaultHintShader(glShader::any_light_texture);
    scene.inForward(window);

    //UI----------------------
    GlRender::bind(ui);
    GlRender::setClearColor(0.f,0.f,0.f,0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    scene.inUI(ui);

    //Постобработка - гамма коррекция----------------------
    GlRender::unbind();
    Blend::Enable(true);
    Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
    Depth::Enable(false);
    glClear(GL_COLOR_BUFFER_BIT);

    //if (event.f.shadow_view) filter.displayRed(scene.light.getDirLight().getShadowMap(), event.f.shadow_level);
    ////else 
    filter.drawExposure(frame.getTexture());
    //ui render
    if (event.f.shadow_view) display(ui.getTexture());
    //
    window.display();
}
//