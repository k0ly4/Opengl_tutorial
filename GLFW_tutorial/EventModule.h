#ifndef EVENT_MODULE_H
#define EVENT_MODULE_H

#include "CascadedShadow.h"
#include "Input.h"

#include "GameModules.h"


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
    }f;

    void update(float time, RenderWindow& window, GraphicPipeline& graphic, Scene& scene);

private:

    void move_camera(float time, Scene& scene);
	Event event;
};

#endif