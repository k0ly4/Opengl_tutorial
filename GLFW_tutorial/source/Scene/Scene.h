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
#include "Scene/LightManager.h"
#include "Game/Voxels/World.h"
#include "Game/Entities/Player.h"

/// <summary>
/// Scene
/// </summary>
class Scene :public RenderScene {
public:

	Scene():
		cube(glShader::any),
		cube2(glShader::any),
		wall(glShader::any),
		player(texCowBoy) {
		
	}
	void initialize(RenderWindow& window) {
		initialize3DScene(window);
		initializeUI(window);
	}
	void inGBuffer(RenderTarget& target);
	void inForward(RenderTarget& target);
	void inUI(RenderTarget& target);
	void inShadowMap(RenderTarget& target, glShader::Object shader);
	void upSizeViews(const glm::ivec2& size) {
		camera.setProjection((float)size.x / (float)size.y, GAME::PROJECTION.fov, GAME::PROJECTION.near, GAME::PROJECTION.far);
		view2D.setProjection(FloatRect(0.f, 0.f, size));
	}
//protected:
//
//	friend class Engine;
	//Texture
	Texture2D texCowBoy;
	Texture2D texError;
	Texture2D filin;
	//Object3D
	LightSystem light;
	Cube cube;
	Cube cube2;
	Cube wall;
	Sphere sphere;
	std::vector<Drawable*>gBufferObjects;
	World world;
	//Object2D
	Sprite sCowBoy;
	Sprite sError;
	//Views
	Camera camera;
	View2D view2D;
	Player player;
	std::vector<Drawable*>shadowObjects;
	std::vector<Drawable*>forwardObjects;

private:

	void initialize3DScene(RenderWindow& window);
	void initializeUI(RenderWindow& window);

};
#endif