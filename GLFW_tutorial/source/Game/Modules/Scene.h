#ifndef SCENE_H
#define SCENE_H

#include "UI/UiScene.h"
#include "Scene/Scene3D.h"
#include "Game/Lib/GameModules.h"

class EventModule;
class GraphicPipeline;
/// <summary>
/// Scene
/// </summary>
class GlobalScene{
public:

	GlobalScene():
		ui(&sc3d),
		sc3d(&ui)
	{}
	void initialize(RenderWindow& window, GraphicPipeline* graphic, EventModule* event) {
		ui.init(window, graphic,event,this);
		sc3d.init(window);
	}
	
	void upSizeViews(const glm::ivec2& size) {
		sc3d.camera.setProjection((float)size.x / (float)size.y, sSetup::camera_proj.fov, sSetup::camera_proj.near, sSetup::camera_proj.far);
		ui.view2D.setProjection(FloatRect(0.f, 0.f, size));
	}

	UiScene ui;
	Scene3D sc3d;

private:

};
#endif