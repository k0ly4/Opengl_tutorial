#ifndef GAME_UI_H
#define GAME_UI_H

#include "UI/Sprite.h"
#include "Game/Voxels/World.h"
#include "Game/Entities/Player.h"
#include "UI/Shape.h"
#include "Scene/Text.h"
#include "Graphic/ContextTexture.h"
class EventModule;
class GraphicPipeline;
class GlobalScene;
//GameUi -----------------------------------------------------
class GameUi
{
public:
	GameUi():player(0) {}
	void init(EventModule* event, GraphicPipeline* graphic, GlobalScene* scene);
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
	EventModule* event;
	GraphicPipeline* graphic;
	GlobalScene* scene;
	RectangleShape back;
};
#endif
