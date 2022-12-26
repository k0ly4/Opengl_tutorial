#ifndef SCENE_H
#define SCENE_H

#include "Scene/Camera.h"
#include "Scene/DeclaredObjects.h"
#include "Scene/Shape.h"
#include "Graphic/RenderWindow.h"
#include "Ui/Sprite.h"
#include "Scene/Quad.h"
#include "Game/Lib/GameModules.h"
#include "Scene/Text.h"
#include "Scene/Sphere.h"
#include "Game/Voxels/World.h"
#include "Game/Entities/Player.h"
#include "Game/Interface/GameUi.h"
/// <summary>
/// Scene
/// </summary>
class Scene:public RenderScene{
public:

	Scene():
		player(texCowBoy) {
		gameUi.setPlayer(&player);
	}

	void initialize(RenderWindow& window) {
		initialize3DScene(window);
		initializeUI(window);
	}

	void inGBuffer(RenderTarget& target);

	void inForward(RenderTarget& target);

	void inUI(RenderTarget& target);
	
	void upSizeViews(const glm::ivec2& size) {
		
		camera.setProjection((float)size.x / (float)size.y, sSetup::camera_proj.fov, sSetup::camera_proj.near, sSetup::camera_proj.far);
		view2D.setProjection(FloatRect(0.f, 0.f, size));
	}

	//устаревший
	void inShadowMap(RenderTarget& target, glShader::Object shader) {}

//protected:
//
//	friend class Engine;
	//Texture
	Texture2D texCowBoy;
	Texture2D texError;
	Texture2D filin;
	//Object3D
	std::vector<Drawable*>gBufferObjects;
	World world;
	//Object2D
	Sprite sError;
	//Views
	Camera camera;
	View2D view2D;
	Player player;
	GameUi gameUi;
	std::vector<Drawable*>shadowObjects;
	std::vector<Drawable*>forwardObjects;

private:

	void initialize3DScene(RenderWindow& window);
	void initializeUI(RenderWindow& window);

};
#endif