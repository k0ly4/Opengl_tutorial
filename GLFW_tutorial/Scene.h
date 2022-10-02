#ifndef SCENE_H
#define SCENE_H

#include"Camera.h"
#include"Text.h"
#include "DeclaredObjects.h"
#include "Shape.h"
#include "RenderWindow.h"
#include"Sprite.h"
#include "GBuffer.h"

#include "GameModules.h"

/// <summary>
/// Scene
/// </summary>
class Scene :public RenderScene {
public:

	Scene() {}
	void initialize(RenderWindow& window) {
		initialize3DScene(window);
		initializeUI(window);
	}
	void inGBuffer(RenderTarget& target);
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
	gbCube plane;
	gbCube cube; 
	gbCube cube2;
	gbCube wall;
	gbPlane plane2;
	std::vector<Drawable*>gBufferObjects;
	//Object2D
	Sprite sCowBoy;
	Sprite sError;
	//Views
	Camera camera;
	View2D view2D;
private:

	void initialize3DScene(RenderWindow& window);
	void initializeUI(RenderWindow& window);

};
#endif