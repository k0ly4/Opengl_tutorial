#ifndef GAME_UI_H
#define GAME_UI_H
#include "UI/Sprite.h"
#include "Game/Voxels/VoxelAtlas.h"
#include "Game/Entities/Player.h"
//GameUi -----------------------------------------------------
class GameUi
{
public:
	GameUi() {
		activeBlock.setTexture(VoxelPack::get()->getTexture());
		activeBlock.setPosition(glm::vec2(0.f));
	}
	inline void setPlayer(Player* player_) { player = player_; }
	inline void draw(RenderTarget& target) {
		glm::vec2 size = glm::vec2(VoxelPack::get()->getTexture().getSize());
		glm::vec2 pos(VoxelPack::get(player->input.curVoxel, Side::top));
		pos = glm::vec2(pos.x, - pos.y);
		FloatRect rect(pos * size, glm::vec2(VoxelPack::get()->getVoxelSize()) * size);
		activeBlock.setTextureRect(rect);
		target.draw(activeBlock);
	}
	
private:

	Player* player;
	Sprite activeBlock;

};
#endif
