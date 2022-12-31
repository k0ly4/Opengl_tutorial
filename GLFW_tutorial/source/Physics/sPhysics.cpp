#include "sPhysics.h"
#include"Scene/Scene.h"

void PhysicsModule::init(GlobalScene& scene) {
	core.setGravity(glm::vec3(0.f, -80.f, 0.f));
	core.setWorld(&scene.sc3d.world);
}

void PhysicsModule::update(float time, GlobalScene& scene) {
	core.solveWorld(time);
	core.debugSolve(time, *scene.sc3d.player.hitbox);
}