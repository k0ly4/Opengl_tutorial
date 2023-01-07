#include "GraphicModule.h"

#include "Input/EventModule.h"
#include "Scene/Scene.h"

/// <summary>
/// GraphicPipeline
/// </summary>
/// 
inline void display(const Texture2D& texture, float exposure) { Debugger::displayExposure(texture, exposure); }
inline void display(const Texture2D& texture) { Debugger::display(texture); }
void GraphicPipeline::initialize(RenderWindow& window) {

    frame.create(window.getSize(), TextureData(GL_RGBA16F, GL_RGBA, GL_NEAREST));
  /*  gBuffer.create(window.getSize());*/
    ui.create(window.getSize(), TextureData(GL_RGBA, GL_RGBA, GL_NEAREST));
}

void GraphicPipeline::render(RenderWindow& window, GlobalScene& sc, EventModule& event) {
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
    GlRender::setClearColor(sc.sc3d.world.weather.colorSky());
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    }
    //Forward render ----------------------
    window.setDefaultHintShader(glShader::any_light_texture);
    sc.sc3d.inForward(window);

    //UI----------------------
    GlRender::bind(ui);
    GlRender::setClearColor(clearUI);
    glClear(GL_COLOR_BUFFER_BIT);
    sc.ui.inRender(ui);

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
    if (event.f.showUi) display(ui.getTexture());
    //
    window.display();
}
//