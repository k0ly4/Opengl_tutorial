#include "Scene.h"

#include "GraphicModule.h"
#include "EventModule.h"

Scene::Scene(RenderWindow& window) {
	cube.setPosition(glm::vec3(10.f));
	cube.setColor(glm::vec3(1.f));
	plane.setPosition(glm::vec3(5.f));
	plane.setScale(glm::vec3(20.f, 0.5f, 20.f));
	plane.setColor(glm::vec3(0.9f));

	light.add(PointLight(glm::vec3(1.f), glm::vec3(25.f, 10.f, 25.f), glm::vec2(0.032f, 0.09f)), &camera);
	light.getDirLight() = DirectionLight(glm::vec3(0.9f), glm::vec3(1.f, 1.f, 0.f));
	light.getDirLight().createMap(1024, 1024, camera);
	light.setAmbientFactor(0.05f);

	camera.setProjection(GAME::PROJECTION);
	camera.cur_pos_mouse = Mouse::getPosition();

	ImageLoader::flipVerticallyOnLoad(1);
	filin.loadFromFile("asset\\image\\favor.jpg");
}

void Scene::inGBuffer(RenderTarget& target) {
	target.draw(plane);
	target.draw(cube);
}

void Scene::inShadowMap(RenderTarget& target, glShader::Object shader) {

	target.draw(cube, shader);
	target.draw(plane, shader);
}