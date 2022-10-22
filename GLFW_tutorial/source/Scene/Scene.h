#ifndef SCENE_H
#define SCENE_H

#include "Scene/Camera.h"
#include "Scene/DeclaredObjects.h"
#include "Scene/Shape.h"
#include "Graphic/RenderWindow.h"
#include "Ui/Sprite.h"
#include "Scene/Quad.h"
#include "Game/GameModules.h"
#include "Scene/Text.h"
#include "Scene/Sphere.h"
#include "Scene/LightManager.h"
#include "Game/World.h"
#include "Game/Player.h"

/// <summary>
/// Scene
/// </summary>
class Scene :public RenderScene {
public:

	Scene() {	}
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
	gbCube cube; 
	gbCube cube2;
	gbCube wall;
	Sphere sphere;
	std::vector<Drawable*>gBufferObjects;
	World world;
	//Object2D
	Sprite sCowBoy;
	Sprite sError;
	Font font;
	Text text2;
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