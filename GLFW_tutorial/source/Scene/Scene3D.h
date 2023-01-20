#ifndef SCENE_3D_H
#define SCENE_3D_H

#include "Scene/Camera.h"
#include "Graphic/RenderWindow.h"
#include "Game/Voxels/World.h"
#include "Game/Entities/Player.h"
#include "Math/Frustum.h"
class UiScene;
class Scene3D
{
public:

	Scene3D(UiScene* ui_);
	void init(RenderWindow & window);
	inline void forwardSetupContext() {
		Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
		Blend::Enable(true);
		Depth::Enable(true);
		CullFace::Mode(CullFace::Back);
	}
	void inForward(RenderTarget& target);
	void inGBuffer(RenderTarget& target);

	Player player;
	//Texture
	Texture2D filin;
	//Object3D
	std::vector<Drawable*>gBufferObjects;
	World world;
	//Views
	Camera camera;

	std::vector<Drawable*>shadowObjects;
	std::vector<Drawable*>forwardObjects;
	UiScene* ui;
	
private:
	
};
#endif
