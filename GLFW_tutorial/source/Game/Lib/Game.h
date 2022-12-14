#ifndef GAME_Hpp
#define GAME_Hpp

#include "Math/Perspective.h"

class sSetup {
public:

    static const glm::vec3 UP;
    static const glm::ivec2 render_size;
    static const Perspective camera_proj;
    static const glm::vec3 start_pos;
    static const char* title;
    static int distance_render;
    static constexpr glm::vec3 getBeginPos() { return glm::ivec3(1100, 100, 1100); }

private:

    sSetup() {}
    ~sSetup() {}

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



