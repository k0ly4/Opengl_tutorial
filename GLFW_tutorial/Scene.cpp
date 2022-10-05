#include "Scene.h"

#include "GraphicModule.h"
#include "EventModule.h"

void Scene::initialize3DScene(RenderWindow& window) {
	wall.setPosition(glm::vec3(6.f));
	wall.setScale(glm::vec3(1.f, 3.f, 3.f));
	wall.setColor(Color::RED);

	cube.setPosition(glm::vec3(10.f));
	cube.setColor(glm::vec3(1.f));
	cube.setTexture(filin);
	cube.setRotate(Angle3D(20.f, glm::vec3(1.f, 0.5f, 0.5f)));

	cube2.setPosition(glm::vec3(11.f, 6.6f, 13.f));
	cube2.setTexture(filin);

	plane.setPosition(glm::vec3(5.f));
	plane.setScale(glm::vec3(20.f, 0.5f, 20.f));
	plane.setColor(glm::vec3(0.9f));
	/// <attenuation>
	/// glm::vec2(0.032f, 0.09f))
	/// </summary>

	light.add(PointLight(glm::vec3(0.5f), glm::vec3(5.f, 13.f, 5.f), glm::vec2(0.032f, 0.09f)), &camera);
	light.getDirLight().setColor(glm::vec3(0.2f));
	light.getDirLight().setDirection(glm::vec3(1.f, 1.f, 0.f));
	light.getDirLight().setSizeMap(glm::ivec2(1024));
	light.setAmbientFactor(0.01f);
	std::cout << light.getPoint(0).getRadius();
	plane2.setColor(Color::GREEN);
	plane2.setPosition(glm::vec3(15.f, 7.f, 10.f));
	plane2.setScale(glm::vec3(5.f));

	camera.setProjection(GAME::PROJECTION);
	camera.cur_pos_mouse = Mouse::getPosition();

	ImageLoader::flipVerticallyOnLoad(1);
	filin.getPath("asset\\image\\favor.jpg");

	//gBufferObjects.push_back(&plane2);
	gBufferObjects.push_back(&plane);
	gBufferObjects.push_back(&wall);
	gBufferObjects.push_back(&cube);
	gBufferObjects.push_back(&cube2);
}

void Scene::initializeUI(RenderWindow& window) {
	view2D.setProjection(FloatRect(0.f, 0.f, window.getSize()));
	//view2D.setProjection(Box(0.f, window.getSize().x, 0.f, window.getSize().y, -1.f, 1.f));
	texCowBoy.getPath("asset\\image\\The fastest camp in the west.png");
	texError.getPath("asset\\image\\FUrfZuZXoAEnZBE.jpg");

	sError.setTexture(texError);
	sError.setPosition(50.f, 50.f);
	sError.setOrigin(sError.getTextureRect().w/2.f, sError.getTextureRect().h/2.f);

	sCowBoy.setTexture(texCowBoy);
	sCowBoy.setPosition(300.f, 200.f);
	

	font.load("asset\\font\\UbuntuMono-R.ttf");
	
	sCowBoy.setTexture(font.getMapGlyphs(20).getTexture());
	//sError.setTexture(font.getMapGlyphs(100).getTexture());
	text.setFont(font);
	text.setString(L"KOlya is the best");
	text.setPosition(glm::vec2(20.f));
	text.setSizeFont(20);
}


/// <summary>
/// Render
/// </summary>

void Scene::inGBuffer(RenderTarget& target) {
	Blend::Enable(false);
	GlRender::DepthTest(true);

	CullFace::Enable(false);
	target.draw(plane2);

	CullFace::Enable(true);
	CullFace::Mode(CullFace::Back);
	for (size_t i = 0; i < gBufferObjects.size(); i++) {
		target.draw(*gBufferObjects[i]);
	}
}

void Scene::inShadowMap(RenderTarget& target, glShader::Object shader) {

	Blend::Enable(false);
	GlRender::DepthTest(true);

	CullFace::Enable(false);
	target.draw(plane2, shader);
	
	CullFace::Enable(true);
	CullFace::Mode(CullFace::Front);
	for (size_t i = 0; i < gBufferObjects.size(); i++) {
		target.draw(*gBufferObjects[i], shader);
	}
	
}

void Scene::inUI(RenderTarget& target) {
	target.setView(view2D);

	CullFace::Enable(false);
	GlRender::DepthTest(false);
	Blend::Enable(true);
	target.draw(sError);
	target.draw(sCowBoy);
	target.draw(text);
}