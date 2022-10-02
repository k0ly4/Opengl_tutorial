#include "Scene.h"

#include "GraphicModule.h"
#include "EventModule.h"

void Scene::initialize(RenderWindow& window) {
	wall.setPosition(glm::vec3(6.f));
	wall.setScale(glm::vec3(1.f, 3.f, 3.f));
	wall.setColor(Color::RED);

	cube.setPosition(glm::vec3(10.f));
	cube.setColor(glm::vec3(1.f));
	cube.setTexture(filin);
	cube.setRotate(Angle3D(20.f, glm::vec3(1.f, 0.5f, 0.5f)));

	cube2.setPosition(glm::vec3(11.f,6.6f,13.f));
	cube2.setTexture(filin);

	plane.setPosition(glm::vec3(5.f));
	plane.setScale(glm::vec3(20.f, 0.5f, 20.f));
	plane.setColor(glm::vec3(0.9f));
	/// <attenuation>
	/// glm::vec2(0.032f, 0.09f))
	/// </summary>
	
	light.add(PointLight(glm::vec3(0.9f), glm::vec3(5.f, 13.f, 5.f), glm::vec2(0.032f, 0.09f)), &camera);
	light.getDirLight().setColor(glm::vec3(0.2f));
	light.getDirLight().setDirection(glm::vec3(1.f, 1.f, 0.f));
	light.getDirLight().setSizeMap(glm::ivec2(1024));
	light.setAmbientFactor(0.01f);

	plane2.setColor(Color::GREEN);
	plane2.setPosition(glm::vec3(15.f,7.f,10.f));
	plane2.setScale(glm::vec3(5.f));

	camera.setProjection(GAME::PROJECTION);
	camera.cur_pos_mouse = Mouse::getPosition();

	ImageLoader::flipVerticallyOnLoad(1);
	filin.loadFromFile("asset\\image\\favor.jpg");

	gBufferObjects.push_back(&plane2);
	gBufferObjects.push_back(&plane);
	gBufferObjects.push_back(&wall);
	gBufferObjects.push_back(&cube);
	gBufferObjects.push_back(&cube2);
}

void Scene::inGBuffer(RenderTarget& target) {
	CullFace::Enable(false);
	target.draw(plane2);

	CullFace::Enable(true);
	CullFace::Mode(CullFace::Back);
	target.draw(plane);
	for (size_t i = 0; i < gBufferObjects.size(); i++) {
		target.draw(*gBufferObjects[i]);
	}
}

void Scene::inShadowMap(RenderTarget& target, glShader::Object shader) {
	CullFace::Enable(false);
	target.draw(plane2, shader);

	CullFace::Enable(true);
	CullFace::Mode(CullFace::Front);
	for (size_t i = 0; i < gBufferObjects.size(); i++) {
		target.draw(*gBufferObjects[i], shader);
	}
	
}