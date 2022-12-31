#ifndef EVENT_MODULE_H
#define EVENT_MODULE_H

#include "Input/Input.h"
#include "Input/InputHandler.h"
#include "Game/Lib/GameModules.h"


/// <summary>
/// EventModule
/// </summary>
class EventModule {

public:

    struct TempFlags {
        bool checkCollision = 0;
        bool showUi = 1;
        bool showDebugInfo = 1;
    }f;

    void initialize(GlobalScene& scene);

    void update(float time, RenderWindow& window, GraphicPipeline& graphic, GlobalScene& scene);

private:

    void inputDisabledCursor(Event& event, GlobalScene& scene, GraphicPipeline& graphic);
	Event event;
    InputHandler inputMain;
};

#endif