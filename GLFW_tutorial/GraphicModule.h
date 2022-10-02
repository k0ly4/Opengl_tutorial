#ifndef GRAPHIC_MODULE_H
#define GRAPHIC_MODULE_H

#include "RenderWindow.h"
#include<list>
#include"Sprite.h"
#include "GBuffer.h"
#include "PostEffects.h"

#include "GameModules.h"

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
    void render(RenderWindow& window, Scene& scene, EventModule& event);
//
//protected:

    FrameBuffer frame;
    GBuffer gBuffer;
    Filter filter;
};

#endif

