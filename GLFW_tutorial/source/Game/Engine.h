#ifndef ENGINE_H
#define ENGINE_H

#include "Game/GameModules.h"
#include "Input/EventModule.h"
#include "Graphic/GraphicModule.h"
#include "Scene/Scene.h"
#include "System/Clock.h"
RenderWindow window;

class Engine  {

    void setupWindow() {
        window.create(GAME::RENDER_SIZE, u8"Коля лучший");
       
        window.setView(scene.camera);

        ImageLoader::flipVerticallyOnLoad(false);
        auto resource = ImageLoader::getSTBI("asset\\image\\favor.jpg",4);
        window.setIcon(resource->getImage());

        window.setFrameInterval(1);
        window.setLimitSizeable(true, glm::ivec2(GLFW_DONT_CARE), glm::ivec2(GLFW_DONT_CARE));
        GlRender::setClearColor(Color::ColorU(150, 199, 196));
    }
  
    Scene scene;
    GraphicPipeline graphic;
    EventModule event;

public:

    Engine()
    {
        setupWindow();
        scene.initialize(window);
        graphic.initialize(window);
    }
  
    void update(float time, Clock& clock) {
        event.update(time, window, graphic, scene);;
    }

    void render() {
        graphic.render(window, scene, event);
    }
 
private:
};
#endif