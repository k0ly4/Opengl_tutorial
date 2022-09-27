#include "EventModule.h"

#include "GraphicModule.h"
#include "Scene.h"

/// <summary>
/// update
/// </summary>
/// 
void EventModule::update(float time, RenderWindow& window, GraphicPipeline& graphic, Scene& scene) {
    while (window.pollEvent(event))
    {
        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Escape) {
                window.close();
            }
            else if (event.key.code == Keyboard::LeftControl) {
                f.rawMotionCursor = !f.rawMotionCursor;
                Mouse::setRawMotion(f.rawMotionCursor);
            }
            else if (event.key.code == Keyboard::LeftShift) {
                f.checkCollision = !f.checkCollision;
            }
            else if (event.key.code == Keyboard::F) {
                f.shadow_view = !f.shadow_view;
            }
            else if (event.key.code == Keyboard::T) {

                scene.light.shadow_demonstration = ~(scene.light.shadow_demonstration);
            }
            else if (event.key.code == Keyboard::N) {
                f.shadow_level++;
                if (f.shadow_level >= NUM_CASCADES)f.shadow_level = NUM_CASCADES - 1;
                printf("Shadow_Level:%d\n", f.shadow_level);
            }
            else if (event.key.code == Keyboard::M) {
                f.shadow_level--;
                if (f.shadow_level < 0) f.shadow_level = 0;
                printf("Shadow_Level:%d\n", f.shadow_level);
            }
            else if (event.key.code == Keyboard::E)
            {
                f.debugGbuffer = !f.debugGbuffer;
            }
        }
        else if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Right) {
                scene.camera.cur_pos_mouse = Mouse::getPosition();
            }
            else if (event.mouseButton.button == Mouse::Middle) {
                scene.light.getDirLight().setDirection(-scene.camera.getBasis().front);
            }
        }
        else if (event.type == Event::WindowResized) {
            graphic.gBuffer.setSize(window.getSize());
            graphic.frame.setSize(window.getSize());
        }

    }
    if (Mouse::isAction(Mouse::Right, Key::Press)) {
        Cursor::setMode(Cursor::Disabled);
        scene.camera.mouse_move(Mouse::getPosition());
    }
    else {
        Cursor::setMode(Cursor::Normal);
    }
    move_camera(time, scene);

}

/// <summary>
/// move_camera
/// </summary>
/// 
void EventModule::move_camera(float time, Scene& scene) {
    const float speed = 10.f;
    glm::vec3 vec_move(0.f);

    if (Keyboard::getKey(GLFW_KEY_W).action) {
        vec_move.z = 1;
    }
    else if (Keyboard::getKey(GLFW_KEY_S).action) {
        vec_move.z = -1;
    }
    if (Keyboard::getKey(GLFW_KEY_D).action) {
        vec_move.x = 1;
    }
    else if (Keyboard::getKey(GLFW_KEY_A).action) {
        vec_move.x = -1;
    }

    //  camera.move(glm::vec3(vec_move.x * time * speed, 0.f, vec_move.z * time * speed));
    float s = sqrt(abs(vec_move.z) + abs(vec_move.x));
    if (s > 0.f) {
        s = (speed * time) / s;
        scene.camera.move(glm::vec3(vec_move.x * s, 0.f, vec_move.z * s));
    }

}
