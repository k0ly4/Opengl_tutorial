#include "sPhysics.h"
#include"Scene/Scene.h"

void PhysicsModule::init(Scene& scene) {
	core.setGravity(glm::vec3(0.f, -90.f, 0.f));
	core.setWorld(&scene.world);
}

void PhysicsModule::update(float time, Scene& scene) {
	core.testSolve(time, *scene.player.hitbox);
}