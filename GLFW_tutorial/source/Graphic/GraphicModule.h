#ifndef GRAPHIC_MODULE_H
#define GRAPHIC_MODULE_H

#include "RenderWindow.h"
#include<list>
#include"Ui/Sprite.h"
#include "GBuffer.h"
#include "PostEffects.h"

#include "Game/GameModules.h"

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
    void setBufferFrameSize(const glm::ivec2& size) {
        frame.setSize(size);
        ui.create(size);
        gBuffer.setSize(size);
    }
//
//protected:
    RenderTexture ui;
    FrameBuffer frame;
    GBuffer gBuffer;
    Filter filter;
private:

};

#endif

