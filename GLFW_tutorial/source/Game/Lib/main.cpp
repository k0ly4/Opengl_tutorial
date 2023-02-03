
#include "Game/Lib/Engine.h"
#include "System/ScriptIntegrator.h"
// Константы

int main()
{   
    srand(time(0));
    GlBuffer::init();
    sBuffer::init();
    glShader::init();
    //scripts
    luke::Lua::init();
    sScriptIntegrator::integrateGlobal();
    //engine
    Engine* engine = new Engine;
    engine->launch();
    //free resources
    delete engine;
    sBuffer::free();
    glShader::free();
    glfwTerminate();

    return 0;
}

