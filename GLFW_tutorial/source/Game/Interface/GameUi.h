#ifndef GAME_UI_H
#define GAME_UI_H

#include "UI/Sprite.h"
#include "Game/Voxels/VoxelAtlas.h"
#include "Game/Entities/Player.h"
#include "UI/Shape.h"
#include "Scene/Text.h"

//GameUi -----------------------------------------------------
class GameUi
{
public:
	GameUi():player(0) {}
	 void init(Player* player_);
	inline void draw(RenderTarget& target) {
		drawDebugInfo(target);
	}
	
private:
	glm::vec2 beg = { 10.f,10.f };
	Font font;
	Text text;
	void drawDebugInfo(RenderTarget& target);
	Player* player;
};
#endif
