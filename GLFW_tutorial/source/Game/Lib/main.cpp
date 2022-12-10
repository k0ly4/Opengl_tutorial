
#include "Game/Lib/Engine.h"
#include "System/LuaFIle.h"
// Константы

int main()
{   
    
    srand(time(0));
    GlBuffer::init();
    sBuffer::init();
    glShader::init();
    luascr::Lua::init();
    MasterGeneration::setGenerator(new CustomGenerator("scripts/generator_config.lua"));
    Engine* engine = new Engine;
    engine->launch();
    delete engine;
    sBuffer::free();
    glShader::free();
    glfwTerminate();

    return 0;
}

