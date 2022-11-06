#pragma once

#include "Game.h"

const glm::vec3 GAME::WORLD_UP = glm::vec3(0.f, 1.f, 0.f);
const glm::ivec2 GAME::RENDER_SIZE = glm::ivec2(1280, 800);
const Perspective GAME::PROJECTION = Perspective((float)GAME::RENDER_SIZE.x / (float)GAME::RENDER_SIZE.y, 55.f, 0.1f, 400.0f);
int GAME::distance_render = 4;