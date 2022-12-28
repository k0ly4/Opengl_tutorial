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
	PhysicsSolver() :
		wTime(1 / 2.f),
		rTime(1 / 60.f)
	{}
	void setWorld(World* world_) {
		world = world_;
	}

	void setGravity(const glm::vec3& gravity_) {
		gravity = gravity_;
	}

	void solve(float time, Hitbox& hitbox) {
		time += rTime.rem;
		while (time >= rTime.step) {
			step(hitbox);
			time -= rTime.step;
		}
		rTime.rem = time;
	}

	void solveWorld(float time) {
		time += wTime.rem;
		while (time >= wTime.step) {
			step_world();
			time -= wTime.step;
		}
		wTime.rem = time;
	}

	void debugSolve(float time, Hitbox& hitbox) {
		time += rTime.rem;

		int physStep = time / rTime.step; if (physStep > 5) LOG("Physstep:%d\n", physStep);

		while (time >= rTime.step) {
			step(hitbox);
			time -= rTime.step;
		}
		rTime.rem = time;
	}
private:

	void step(Hitbox& hibox);
	void step_world();
	void modelingLuqid(size_t index, Voxels& voxs, Chunk* chunk);
	glm::vec3 gravity;

	struct UpTime {
		float step;
		float rem = 0.f;
		UpTime() {}
		UpTime(float step_): step(step_){}
	};

	UpTime rTime;
	UpTime wTime;
	World* world;
};
#endif
