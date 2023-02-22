#ifndef UI_SCENE_H
#define UI_SCENE_H

#include "UI/Sprite.h"
#include "Scene/Text.h"
#include "Game/Interface/GameUi.h"
#include "Graphic/RenderWindow.h"
#include "Shape.h"
#include "Graphic/TextureMap.h"
class Scene3D;
class EventModule;
class GraphicPipeline;
class GlobalScene;
class UiScene
{
public:
	UiScene(Scene3D* sc3d_) :main(sc3d_) {}
	void init(RenderWindow& window, GraphicPipeline* graphic, EventModule*event, GlobalScene* scene);
	inline void setContext()noexcept {
		Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
		CullFace::Enable(false);
		Depth::Enable(false);
		Blend::Enable(true);
	}
	void inRender(RenderTarget& target);
	Texture2D texCowBoy;
	Texture2D texError;
	GameUi gameUi;
	View2D view2D;
	Scene3D* main;
	
private:
	EventModule* mEvent;

};
#endif

