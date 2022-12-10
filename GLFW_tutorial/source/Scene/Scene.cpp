
#include "Scene/Scene.h"
#include "Graphic/GraphicModule.h"
#include "Input/EventModule.h"

void Scene::initialize3DScene(RenderWindow& window) {
	player.setCamera(camera);
	player.hitbox->position = sSetup::getBeginPos();
	camera.setPosition(sSetup::getBeginPos());
	camera.setProjection(GAME::PROJECTION);
	camera.cur_pos_mouse = Mouse::getPosition();

	ImageLoader::flipVerticallyOnLoad(1);
	filin.getPath("asset\\image\\favor.jpg");
	world.init(camera);
}

void Scene::initializeUI(RenderWindow& window) {
	view2D.setProjection(FloatRect(0.f, 0.f, window.getSize()));
	//view2D.setProjection(Box(0.f, window.getSize().x, 0.f, window.getSize().y, -1.f, 1.f));
	texCowBoy.getPath("asset\\image\\The fastest camp in the west.png");
	texError.getPath("asset\\image\\FUrfZuZXoAEnZBE.jpg");

	sError.setTexture(texError);
	texError.wrap(TextureWrap::ClampToEdge);
	sError.setPosition(50.f, 50.f);
	sError.setOrigin(sError.getTextureRect().w/2.f, sError.getTextureRect().h/2.f);

	sCowBoy.setTexture(texError);
	sCowBoy.setPosition(700.f, 200.f);
}


/// <summary>
/// Render
/// </summary>

void Scene::inGBuffer(RenderTarget& target) {
	Blend::Enable(false);
	Depth::Enable(true);
	CullFace::Mode(CullFace::Back);

	CullFace::Enable(false);

	CullFace::Enable(true);
	for (size_t i = 0; i < gBufferObjects.size(); i++) {
		target.draw(*gBufferObjects[i]);
	}
}

inline void forwardSetupContext() {
	Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	Blend::Enable(true);
	Depth::Enable(true);
	CullFace::Mode(CullFace::Back);
}

void Scene::inForward(RenderTarget& target) {
	
	forwardSetupContext();

	CullFace::Enable(false);
	target.draw(world.chunks);
	player.cursor.draw(target);
	/*Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	CullFace::Enable(true);*/
	//target.draw(light.getPoints());
	//DrawObject
	//Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	//DrawObject
	//Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
}


//void Scene::inShadowMap(RenderTarget& target, glShader::Object shader) {
//
//	Blend::Enable(false);
//	Depth::Enable(true);
//	CullFace::Mode(CullFace::Back);
//	CullFace::Enable(false);
//	CullFace::Enable(true); 
//}
inline void uiSetupContext() {
	Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	CullFace::Enable(false);
	Depth::Enable(false);
	Blend::Enable(true);
}
void Scene::inUI(RenderTarget& target) {
	target.setView(view2D);
	uiSetupContext();
	target.draw(sCowBoy);
	gameUi.draw(target);
}