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
	void initialize(RenderWindow& window);
	void inGBuffer(RenderTarget& target);

	void inShadowMap(RenderTarget& target, glShader::Object shader);

//protected:
//
//	friend class Engine;

	Texture2D filin;
	LightSystem light;
	gbCube plane;
	gbCube cube; 
	gbCube cube2;
	gbCube wall;
	Camera camera;
	gbPlane plane2;
	std::vector<Drawable*>gBufferObjects;

};
#endif