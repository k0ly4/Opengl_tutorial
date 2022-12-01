#pragma once

#include "Game.h"

const glm::vec3		sSetup::UP = glm::vec3(0.f, 1.f, 0.f);
const glm::ivec2	sSetup::render_size = glm::ivec2(1280, 800);
const Perspective	sSetup::camera_proj =
							Perspective((float)sSetup::render_size.x / (float)sSetup::render_size.y, 55.f, 0.1f, 400.0f);
int					sSetup::distance_render = 4;
const glm::vec3		sSetup::start_pos = glm::vec3(100.f, 60, 100.f);
const char*			sSetup::title = u8"Коля лучший\n";

const glm::vec3 GAME::WORLD_UP = glm::vec3(0.f, 1.f, 0.f);
const glm::ivec2 GAME::RENDER_SIZE = glm::ivec2(1280, 800);
const Perspective GAME::PROJECTION = Perspective((float)GAME::RENDER_SIZE.x / (float)GAME::RENDER_SIZE.y, 55.f, 0.1f, 400.0f);
int GAME::distance_render = 4;
