#ifndef ENGINE_H
#define ENGINE_H

#include "Game/Lib/GameModules.h"
#include "Input/EventModule.h"
#include "Graphic/GraphicModule.h"
#include "Scene/Scene.h"
#include "System/Clock.h"
#include "Physics/sPhysics.h"

#include <thread>
RenderWindow window;

class Engine  {

public:

    Engine()
    {
        setupWindow();
        scene.initialize(window);
        graphic.initialize(window);
        event.initialize(scene);
        physics.init(scene);
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
        window.setView(scene.camera);

        ImageLoader::flipVerticallyOnLoad(false);
        auto resource = ImageLoader::getSTBI("asset\\image\\favor.jpg", 4);
        window.setIcon(resource->getImage());

        window.setFrameInterval(1);
        window.setLimitSizeable(true, glm::ivec2(GLFW_DONT_CARE), glm::ivec2(GLFW_DONT_CARE));
        GlRender::setClearColor(Color::ColorU(150, 199, 196));

    }

    Scene scene;
    GraphicPipeline graphic;
    EventModule event;
    PhysicsModule physics;
    Clock clock;
    std::thread* thread_render;
};
#endif