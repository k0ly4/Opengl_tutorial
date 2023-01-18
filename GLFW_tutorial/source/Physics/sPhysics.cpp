#include "sPhysics.h"
#include"Scene/Scene.h"

void PhysicsModule::init(GlobalScene& scene) {
	//core.setGravity(glm::vec3(0.f, -80.f, 0.f));
	core.init(&scene);
}

void PhysicsModule::update(float time, GlobalScene& scene) {
	core.upTime(time);
	core.solveWorld();
	core.solve(*scene.sc3d.player.hitbox);
}