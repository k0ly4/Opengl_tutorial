#include "Scene3D.h"
#include "UI/UiScene.h"


Scene3D::Scene3D(UiScene* ui_) :
	ui(ui_),
	player(ui_->texCowBoy)
{
}

void Scene3D::init(RenderWindow& window) {
	player.setCamera(camera);
	player.hitbox->position = sSetup::getBeginPos();
	camera.setPosition(sSetup::getBeginPos());
	camera.setProjection(GAME::PROJECTION);
	camera.cur_pos_mouse = Mouse::getPosition();

	ImageLoader::flipVerticallyOnLoad(1);
	filin.getPath("asset\\image\\favor.jpg");
	world.init(camera);
}

void Scene3D::inForward(RenderTarget& target) {

	forwardSetupContext();

	CullFace::Enable(false);
	target.draw(world.chunks);
	player.input.cursor.draw(target);
	/*Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	CullFace::Enable(true);*/
	//target.draw(light.getPoints());
	//DrawObject
	//Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	//DrawObject
	//Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
}

void Scene3D::inGBuffer(RenderTarget& target) {
	Blend::Enable(false);
	Depth::Enable(true);
	CullFace::Mode(CullFace::Back);

	CullFace::Enable(false);

	CullFace::Enable(true);
	for (size_t i = 0; i < gBufferObjects.size(); i++) {
		target.draw(*gBufferObjects[i]);
	}
}