
#include "Scene/Scene.h"
#include "Graphic/GraphicModule.h"
#include "Input/EventModule.h"

void Scene::initialize3DScene(RenderWindow& window) {


	sphere.setRadius(3.0f);
	sphere.setCountSector(8);
	sphere.setCountStack(8);

	sphere.setPosition(glm::vec3(10.f, 10.f, 10.f));
	sphere.setTexture(filin);

	wall.setPosition(glm::vec3(6.f));
	wall.setScale(glm::vec3(1.f, 3.f, 3.f));
	wall.setBaseColor(Color::RED);

	cube.setPosition(glm::vec3(10.f));
	cube.setBaseColor(glm::vec3(1.f));
	cube.setTexture(filin);
	cube.setRotate(AngleAxis(20.f, glm::vec3(1.f, 0.5f, 0.5f)));

	cube2.setPosition(glm::vec3(11.f, 6.6f, 13.f));
	cube2.setTexture(filin);

	

	auto& points = light.getPoints();
	points.push_back(PointLight(glm::vec3(0.5f), glm::vec3(5.f, 13.f, 5.f), glm::vec2(0.032f, 0.09f)), &camera);
	light.getDirs().setColor(glm::vec3(0.1f));
	light.getDirs().setDirection(glm::vec3(1.f, 1.f, 0.f));
	light.getDirs().setSizeMap(glm::ivec2(1024));
	light.setAmbientFactor(0.01f);

	camera.setProjection(GAME::PROJECTION);
	camera.cur_pos_mouse = Mouse::getPosition();

	ImageLoader::flipVerticallyOnLoad(1);
	filin.getPath("asset\\image\\favor.jpg");

	gBufferObjects.resize(2);
	
	gBufferObjects[0] = (&wall);
	gBufferObjects[1] = (&cube);
	//LOG("this is%d exams\n",100u)

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
	

	font.load("asset\\font\\UbuntuMono-R.ttf");
	MapGlyph& map_ = font.getMapGlyphs(20);
	sCowBoy.setTexture(map_.getTexture());
	sError.setTexture(map_.getTexture());
	
	text2.setFont(font);
	text2.setString(L"KOlya is the best\nI love you");
	text2.setPosition(glm::vec2(10.f));
	text2.setSizeFont(20);
	text2.setScale(glm::vec2(7.f));
	text2.setColor(Color::YELLOW);
	
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

void Scene::inForward(RenderTarget& target) {
	Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	Blend::Enable(true);
	Depth::Enable(true);

	CullFace::Mode(CullFace::Back);

	CullFace::Enable(false);
	target.draw(world.chunks);
	sphere.displayLine(target);

	
	Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);

	CullFace::Enable(true);
	target.draw(light.getPoints());
	light.draw(target, camera, sphere);
	Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	light.draw(target,camera,cube2);
	Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
}


void Scene::inShadowMap(RenderTarget& target, glShader::Object shader) {

	Blend::Enable(false);
	Depth::Enable(true);
	CullFace::Mode(CullFace::Back);

	CullFace::Enable(false);
	
	CullFace::Enable(true); 
	target.draw(cube, shader);
	
	target.draw(wall, shader);

	target.draw(sphere, shader);
	target.draw(cube2, shader);
}

void Scene::inUI(RenderTarget& target) {
	target.setView(view2D);

	Blend::Func(Blend::SrcAlpha, Blend::OneMinusSrcAlpha);
	CullFace::Enable(false);
	Depth::Enable(false);
	Blend::Enable(true);
	target.draw(text2);
	target.draw(sCowBoy);

}