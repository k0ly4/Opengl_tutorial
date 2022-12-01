
#include "Game/Lib/Engine.h"
// Константы

int main()
{   
    
    srand(time(0));
    GlBuffer::init();
    sBuffer::init();
    glShader::init();

    Engine* engine = new Engine;
    engine->launch();
    delete engine;
    sBuffer::free();
    glShader::free();
    glfwTerminate();
    return 0;
}

