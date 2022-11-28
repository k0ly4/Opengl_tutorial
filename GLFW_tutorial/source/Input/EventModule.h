#ifndef EVENT_MODULE_H
#define EVENT_MODULE_H

#include "Scene/CascadedShadow.h"
#include "Input/Input.h"
#include "Input/InputHandler.h"
#include "Game/Lib/GameModules.h"


/// <summary>
/// EventModule
/// </summary>
class EventModule {

public:

    struct TempFlags {
        size_t cur_animation = 0;
        int shadow_level = 0;
        bool shadow_view = 0;
        int cur_bone = 0;
        int mode_bone = 0;
        bool rawMotionCursor = 0;
        bool checkCollision = 0;
        bool debugGbuffer = 0;
        float zMult = 10.f;
        bool debugCascadeShadow = 0;
    }f;

    void initialize(Scene& scene);

    void update(float time, RenderWindow& window, GraphicPipeline& graphic, Scene& scene);

private:

    void inputDisabledCursor(Event& event, Scene& scene, GraphicPipeline& graphic);
	Event event;
    InputHandler inputMain;
};

#endif