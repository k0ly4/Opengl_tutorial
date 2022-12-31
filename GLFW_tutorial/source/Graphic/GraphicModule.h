#ifndef GRAPHIC_MODULE_H
#define GRAPHIC_MODULE_H

#include<list>

#include "Graphic/RenderWindow.h"
#include "Graphic/PostEffects.h"

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
    RenderTexture ui;
    FrameBuffer frame;
    Filter filter;
private:

};

#endif

