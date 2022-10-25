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

        else if (event.key.code == Keyboard::LeftShift) {
            f.debugCascadeShadow = !f.debugCascadeShadow;
            graphic.gBuffer.setDebugMode(f.debugCascadeShadow);
        }

   
        else if (event.key.code == Keyboard::F) {
            printf("F\n");
            f.shadow_view = !f.shadow_view;
        }

        else if (event.key.code == Keyboard::T) {

            scene.light.shadow_demonstration = ~(scene.light.shadow_demonstration);
        }

        else if (event.key.code == Keyboard::N) {
            f.shadow_level++;
            if (f.shadow_level >= scene.light.getDirs().getShadowMap().getCount())
                f.shadow_level = scene.light.getDirs().getShadowMap().getCount() - 1;
            printf("Shadow_Level:%d\n", f.shadow_level);
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
    
    }
    else if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Right) {
            scene.player.setVoxel(scene.world.chunks, 1);
        }
        else if (event.mouseButton.button == Mouse::Middle) {
            scene.light.getDirs().setDirection(-scene.camera.getBasis().front);
            scene.light.getPoints()[0].setPosition(scene.camera.getPosition());
            scene.light.lightTest.setDirection(scene.camera);
        }
        else if (event.mouseButton.button == Mouse::Left) {
            scene.player.setVoxel(scene.world.chunks,0);
        }
    }
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
    Cursor::setMode(Cursor::Disabled);
    //Camera
    scene.camera.mouse_move(Mouse::getPosition());
    move_camera(time, scene);
    scene.player.upVoxelCursor(scene.world.chunks);
}

/// <summary>
/// move_camera
/// </summary>
/// 
void EventModule::move_camera(float time, Scene& scene) {
const float speed = 10.f;
glm::vec3 vec_move(0.f);

if (Keyboard::getKey(Keyboard::W).action) {
    vec_move.z = 1;
}
else if (Keyboard::getKey(Keyboard::S).action) {
    vec_move.z = -1;
}
if (Keyboard::getKey(Keyboard::D).action) {
    vec_move.x = 1;
}
else if (Keyboard::getKey(Keyboard::A).action) {
    vec_move.x = -1;
}

//  camera.move(glm::vec3(vec_move.x * time * speed, 0.f, vec_move.z * time * speed));
float s = sqrt(abs(vec_move.z) + abs(vec_move.x));
if (s > 0.f) {
    s = (speed * time) / s;
    scene.camera.move(glm::vec3(vec_move.x * s, 0.f, vec_move.z * s));
}

}
