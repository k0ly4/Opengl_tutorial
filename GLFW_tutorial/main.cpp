
#include "Engine.h"
// Константы

int main()
{   
    
    srand(time(0));
    glShader::init();
    Engine* engine = new Engine;
    Clock clock;
    while (window.isOpen())
    {
        // Обработка ввоа
        float time = clock.restart();
        engine->update(time, clock);
        engine->render();
       // camera.update();
    
    }
    delete engine;
    glShader::free();
    glfwTerminate();
    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение

    // glfw: завершение, освобождение всех ранее задействованных GLFW-ресурсов
   
    return 0;
}

