#ifndef GAME_Hpp
#define GAME_Hpp

#include "Math/Perspective.h"


class GAME {
public:

    static const glm::vec3 WORLD_UP;
    static const glm::ivec2 RENDER_SIZE;
    static const Perspective PROJECTION;

private:
    GAME() {}
    ~GAME() {}
};

#endif



