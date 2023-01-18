#ifndef GAME_UI_H
#define GAME_UI_H

#include "UI/Sprite.h"
#include "Game/Voxels/World.h"
#include "Game/Entities/Player.h"
#include "UI/Shape.h"
#include "Scene/Text.h"
#include "Graphic/ContextTexture.h"

//GameUi -----------------------------------------------------
class GameUi
{
public:
	GameUi():player(0) {}
	 void init(Player* player_,World* world_);
	inline void draw(RenderTarget& target) {
		drawDebugInfo(target);
	}
	
private:
	World*world;
	glm::vec2 beg = { 0.f,0.f };
	std::wstring info;
	Font font;
	Text text;
	void drawDebugInfo(RenderTarget& target);
	Player* player;
	RectangleShape back;
};
#endif
