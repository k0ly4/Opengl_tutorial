#ifndef PHYSICS_SOLVER_H
#define PHYSICS_SOLVER_H

#include "Hitbox.h"
#include "Game/Voxels/World.h"

//PhysicsSolver----------------------------------------------
/// <summary>
/// ѕока что основной класс на который нужно жаловатьс€ за ошибки в физической имитации
/// </summary>
/// 
class PhysicsSolver
{
public:

	void setWorld(World* world_) {
		world = world_;
	}

	void setGravity(const glm::vec3& gravity_) {
		gravity = gravity_;
	}

	void solve(float time, Hitbox& hitbox) {
		time += remTime;
		while (time >= timeStep) {
			step(hitbox);
			time -= timeStep;
		}
		remTime = time;
	}
	void testSolve(float time, Hitbox& hitbox) {
		time += remTime;
		while (time >= timeStep) {
			test_step(hitbox);
			time -= timeStep;
		}
		remTime = time;
	}
private:

	void step(Hitbox& hibox);
	void test_step(Hitbox& hibox);
	World* world;

	glm::vec3 gravity;

	float timeStep = 1 / 60.f;;
	float remTime = 0.f;
};
#endif
