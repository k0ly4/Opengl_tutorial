#ifndef GRAPHIC_MODULE_H
#define GRAPHIC_MODULE_H

#include<list>

#include "Graphic/RenderWindow.h"
#include "Graphic/PostEffects.h"
#include "System/Clock.h"
#include"Ui/Sprite.h"
#include "Game/Lib/GameModules.h"

/// <summary>
/// GraphicPipeline
/// </summary>
class GraphicPipeline :public FrameBuffer
{
public:
    GraphicPipeline(){
        filter.setExposure(2.2f);
        clearUI =   {   0.f,0.f,0.f,0.f };
        clearMain = {   0.7f, 0.7f, 1.f,1.f };

    }
    void initialize(RenderWindow& window);
    void render(RenderWindow& window, GlobalScene& scene, EventModule& event);
    void setBufferFrameSize(const glm::ivec2& size) {
        frame.setSize(size);
        ui.create(size);
       //gBuffer.setSize(size);
    }
//
//protected:
    Color clearUI, clearMain;
    RenderTexture ui;
    FrameBuffer frame;
    Filter filter;
    float time;
    float fps;
private:
    
    Clock clock;

};

#endif

