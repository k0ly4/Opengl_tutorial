#include "EventModule.h"

#include "GraphicModule.h"
#include "Scene.h"

/// <summary>
/// update
/// </summary>
/// 
void EventModule::inputNormalCursor(Event& event, Scene& scene){
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Backspace) {


            scene.text2.input(event.text.unicode, 1);

        }
        else if (event.key.code == Keyboard::Enter) {

            scene.text2.input(event.key.code, 1);

        }

        else if (event.key.code == Keyboard::LeftBracket) {
            auto& visual = scene.text2.getFormEffects();
            visual.thickness_ = visual.thickness_ - 0.02f;
        }
        else if (event.key.code == Keyboard::RightBracket) {
            auto& visual = scene.text2.getFormEffects();
            visual.thickness_ = visual.thickness_ + 0.02f;
        }

        else if (event.key.code == Keyboard::Num0) {
            auto& visual = scene.text2.getFormEffects();
            visual.softness_ = visual.softness_ - 0.5f;
        }
        else if (event.key.code == Keyboard::Num1) {
            auto& visual = scene.text2.getFormEffects();
            visual.softness_ = visual.softness_ + 0.5f;
        }


        else if (event.key.code == Keyboard::Num2) {
            auto& visual = scene.text2.getOutlineEffects();
            visual.thickness_ = visual.thickness_ - 0.02f;
        }
        else if (event.key.code == Keyboard::Num3) {
            auto& visual = scene.text2.getOutlineEffects();
            visual.thickness_ = visual.thickness_ + 0.02f;
        }

        else if (event.key.code == Keyboard::Num4) {
            auto& visual = scene.text2.getOutlineEffects();
            visual.softness_ = visual.softness_ - 0.5f;
        }
        else if (event.key.code == Keyboard::Num5) {
            auto& visual = scene.text2.getOutlineEffects();
            visual.softness_ = visual.softness_ + 0.5f;
        }

    }
        else if (event.type == Event::TextEntered) {

            if (Cursor::getMode() == Cursor::Normal) {
                if (event.text.unicode == ']' || event.text.unicode == '[' || (event.text.unicode>='0' && event.text.unicode <='9'))return;
                printf("TextEntered::code:%d", event.text.unicode);
                scene.text2.input(event.text.unicode, 0);
            }

        }

}
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

        else if (event.key.code == Keyboard::U) {
            f.zMult -= 1.f;
            scene.light.getDirs().setMultShadow(f.zMult);
        }

        else if (event.key.code == Keyboard::Y) {
            f.zMult += 1.f;
            scene.light.getDirs().setMultShadow(f.zMult);
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
    
    }
    else if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Right) {
            scene.camera.cur_pos_mouse = Mouse::getPosition();
        }
        else if (event.mouseButton.button == Mouse::Middle) {
            scene.light.getDirs().setDirection(-scene.camera.getBasis().front);
            scene.light.getPoints()[0].setPosition(scene.camera.getPosition());
            scene.light.lightTest.setDirection(scene.camera);
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
       
        if (Cursor::getMode() == Cursor::Normal) {
            inputNormalCursor(event, scene);
        }
        else {
            inputDisabledCursor(event, scene,graphic);
        }
            
        
  
    }

    //Cursor
    if (Mouse::isAction(Mouse::Right, Key::Press)) {
        Cursor::setMode(Cursor::Disabled);
        scene.camera.mouse_move(Mouse::getPosition());
    }
    else {
        Cursor::setMode(Cursor::Normal);
    }
    //Camera
    if (Cursor::getMode() == Cursor::Disabled)
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
