#ifndef GAME_Hpp
#define GAME_Hpp

#include "Math/Perspective.h"

class sGameSetup {
public:

    static const glm::vec3 WORLD_UP;
    static const glm::ivec2 RENDER_SIZE;
    static const Perspective PROJECTION;
    static const glm::vec3 start_pos;
    static int distance_render;
private:
    sGameSetup() {}
    ~sGameSetup() {}

};
class GAME {
public:

    static const glm::vec3 WORLD_UP;
    static const glm::ivec2 RENDER_SIZE;
    static const Perspective PROJECTION;
    static int distance_render;
private:
    GAME() {}
    ~GAME() {}
};

#endif



