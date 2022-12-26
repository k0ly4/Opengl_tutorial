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
		activeBlock.setScale(glm::vec2(2.f));
	}
	inline void setPlayer(Player* player_) { player = player_; }
	inline void draw(RenderTarget& target) {
		glm::vec2 size = glm::vec2(VoxelPack::get()->getTexture().getSize());
		glm::vec2 norm(VoxelPack::get(player->input.curVoxel, Side::top));
		glm::vec2 pos(
			glm::vec2(norm.x, 1.f - norm.y-VoxelPack::get()->getNormalizeSizeVoxel()) * size
		);
		FloatRect rect(pos, glm::vec2(VoxelPack::get()->getSizeVoxel()));
		activeBlock.setTextureRect(rect);
		target.draw(activeBlock);
	}
	
private:

	Player* player;
	Sprite activeBlock;

};
#endif
