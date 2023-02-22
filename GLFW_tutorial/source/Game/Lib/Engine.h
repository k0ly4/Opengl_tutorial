#ifndef ENGINE_H
#define ENGINE_H

#include "Game/Lib/GameModules.h"
#include "Game/Modules//EventModule.h"
#include "Game/Modules//GraphicModule.h"
#include "Game/Modules//Scene.h"
#include "System/Clock.h"
#include "Physics/sPhysics.h"
#include "Game/Entities/ProcessQueue.h"
#include <thread>
RenderWindow window;

class Engine  {

public:

    Engine()
    {
        MasterGeneration::setGenerator(new CustomGenerator("scripts/i_generator.lua"));
        setupWindow();
        procQueue.launch();
        scene.initialize(window,&graphic,&event);
        graphic.initialize(window);
        event.initialize(scene);
        physics.init(scene);
        sRender::Line::Width(5.f);
  
      //  MasterGeneration::getBiomMap().mapToPNG("screenshots\\biom_map.png");
    }

    void launch() {
       GLFW::setActive(false);
       thread_render = new std::thread(&Engine::render, this);
       
       clock.restart();
       update();
    }
private:

    void update() {
        float time;
        while (window.isOpen()) {
            time = (float)clock.restart();
            event.update(time, window, graphic, scene);
            physics.update(time, scene);
        }
        procQueue.kill();
        thread_render->join();
        delete thread_render;
    }

    void render() {
        GLFW::setActive(true);
        while (window.isOpen()) {
            graphic.render(window, scene, event);
        }
    }

    void setupWindow() {

        window.create(sSetup::render_size, sSetup::title);
        window.setView(scene.sc3d.camera);

        ImageLoader::setFlipOnLoad(false);
        auto resource = ImageLoader::getSTBI("asset\\image\\favor.jpg", 4);
        window.setIcon(resource->getImage());

        window.setFrameInterval(1);
        window.setLimitSizeable(true, glm::ivec2(GLFW_DONT_CARE), glm::ivec2(GLFW_DONT_CARE));
        sRender::Clear::setColor(Color::ColorU(150, 199, 196));

    }

    ProcessQueue procQueue;
    GlobalScene scene;
    GraphicPipeline graphic;
    EventModule event;
    PhysicsModule physics;
    Clock clock;
    std::thread* thread_render;

};
#endif