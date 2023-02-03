#include "Game/Modules/GraphicModule.h"
#include "Game/Modules/EventModule.h"
#include "Game/Modules/Scene.h"

/// <summary>
/// GraphicPipeline
/// </summary>
/// 
inline void display(const Texture2D& texture, float exposure) { Debugger::displayExposure(texture, exposure); }
inline void display(const Texture2D& texture) { Debugger::display(texture); }
void GraphicPipeline::initialize(RenderWindow& window) {

    frame.create(window.getSize(), TextureData(GL_RGBA16F, GL_RGBA, GL_NEAREST));
    ui.create(window.getSize(), { GL_RGBA, GL_RGBA }, GL_NEAREST);
    clock.restart();
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
  
        Render::bind(frame);
        Render::setClearColor(sc.sc3d.world.weather.colorSky());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.setDefaultHintShader(glShader::any_light_texture);
        sc.sc3d.inForward(window);

        //UI----------------------
        Render::bind(ui);
        Render::setClearColor(clearUI);
        glClear(GL_COLOR_BUFFER_BIT);
        sc.ui.inRender(ui);

        //Постобработка - гамма коррекция----------------------
        Render::unbind();
        Blend::Enable(true);
        Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
        Depth::Enable(false);
        glClear(GL_COLOR_BUFFER_BIT);

        //if (event.f.shadow_view) filter.displayRed(scene.light.getDirLight().getShadowMap(), event.f.shadow_level);
        ////else 
        filter.drawExposure(frame.getTexture());
        //ui render
        if (event.f.showUi) display(*ui.texture());
        //
        window.display();
        time = clock.restart();;
        fps = (3.f * fps + 1.f / time) / 4.f;

  
}
//