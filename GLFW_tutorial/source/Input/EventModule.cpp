#include "Input/EventModule.h"
#include "Graphic/GraphicModule.h"
#include "Scene/Scene.h"

/// <summary>
/// update
/// </summary>
/// 
void EventModule::inputDisabledCursor(Event& event,Scene& scene,GraphicPipeline& graphic) {
    if (event.type == Event::KeyPressed) {

        if (event.key.code == Keyboard::LeftControl) {
            f.rawMotionCursor = !f.rawMotionCursor;
            Mouse::setRawMotion(f.rawMotionCursor);
        }

       
        else if (event.key.code == Keyboard::F) {
            printf("F\n");
            f.shadow_view = !f.shadow_view;
        }
       
        else if (event.key.code == Keyboard::M) {
            f.shadow_level--;
            if (f.shadow_level < 0)
                f.shadow_level = 0;

            printf("Shadow_Level:%d\n", f.shadow_level);
        }
        else if (event.key.code == Keyboard::E)
        {
            f.debugGbuffer = !f.debugGbuffer;
        }
        else if (event.key.code == Keyboard::F1)
        {
            scene.world.save();
        }
        else if (event.key.code >= Keyboard::Num0 && event.key.code <= Keyboard::Num9) {
            scene.player.input.setCurVoxel(Voxel(event.key.code - Keyboard::Num0));
        }
        else inputMain.solveKey(event);
    }

    else if (event.type == Event::MouseButtonPressed) {
        inputMain.solveMouse(event);
    }
    else if (event.type == Event::MouseScrolled) {
        inputMain.solveScroll(event);
    }
}

void EventModule::initialize(Scene& scene) {
       sCommandHandler::init(inputMain,&scene, &scene.player);
}

void EventModule::update(float time, RenderWindow& window, GraphicPipeline& graphic, Scene& scene) {
    //PollEvent
    while (window.pollEvent(event))
    {
        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Escape) {
                window.close();
                continue;
            }  
        }
        else if (event.type == Event::WindowResized) {
            graphic.setBufferFrameSize(event.size.size);
            scene.upSizeViews(event.size.size);
            continue;
        }
        inputDisabledCursor(event, scene,graphic);
    }
    //Cursor
    Cursor::setMode(Cursor::Disabled);
    //Player
    scene.player.input.cameraUpdate(Mouse::getPosition());
    scene.player.input.moveUpdate(time);
    scene.player.cursorUpdate(scene.world.chunks);
    //RenderSectorChunks
    scene.world.chunks.setCameraPos(scene.player.getPosition());
}
