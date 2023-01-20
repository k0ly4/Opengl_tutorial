#include "Game/Modules/EventModule.h"
#include "Game/Modules/GraphicModule.h"
#include "Game/Modules/Scene.h"

/// <summary>
/// update
/// </summary>
/// 
void EventModule::inputDisabledCursor(Event& event, GlobalScene& sc,GraphicPipeline& graphic) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::F) {
            f.showUi = !f.showUi;
        }
        else if (event.key.code == Keyboard::F1)
        {
            sc.sc3d.world.save();
        }
        else if (event.key.code >= Keyboard::Num0 && event.key.code <= Keyboard::Num9) {
            sc.sc3d.player.input.setCurVoxel(Voxel(event.key.code - Keyboard::Num0));
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

void EventModule::initialize(GlobalScene& sc) {
       sCommandHandler::init(inputMain,&sc, &sc.sc3d.player);
}

void EventModule::update(float time, RenderWindow& window, GraphicPipeline& graphic, GlobalScene& sc) {
    f.fps = (3.f*f.fps +1.f/time)/4.f;
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
            sc.upSizeViews(event.size.size);
            continue;
        }
        inputDisabledCursor(event, sc,graphic);
    }
    //Cursor
    Cursor::setMode(Cursor::Disabled);
    //Player
    sc.sc3d.player.input.cameraUpdate(Mouse::getPosition());
    sc.sc3d.player.input.moveUpdate(time);
    sc.sc3d.player.cursorUpdate(sc.sc3d.world.chunks);
    //RenderSectorChunks
    sc.sc3d.world.chunks.upView(sc.sc3d.camera);
   
}
