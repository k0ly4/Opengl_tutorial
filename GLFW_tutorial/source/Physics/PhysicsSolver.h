#ifndef PHYSICS_SOLVER_H
#define PHYSICS_SOLVER_H

#include "Hitbox.h"
#include "Game/Voxels/World.h"
//PhysicsSolver----------------------------------------------
/// <summary>
/// ѕока что основной класс на который нужно жаловатьс€ за ошибки в физической имитации
/// </summary>
/// 
/// 
class  GlobalScene;
class PhysicsSolver
{
public:
	PhysicsSolver() :
		gravity(glm::vec3(0.f, -80.f, 0.f)),
		wTime(1 / 2.f),
		rTime(1 / 60.f)
	{}
	void init(GlobalScene* scene_);
	inline void upTime(float time) {
		rTime.up(time);
		if (rTime.count > 5) LOG("rTime:%d\n", rTime.count);
		wTime.up(time);
		if (wTime.count > 5) LOG("wTime:%d\n", wTime.count);
	}

	inline void setGravity(const glm::vec3& gravity_) { gravity = gravity_;}
	inline void solve(Hitbox& hitbox) {	for (size_t i = 0; i < rTime.count; i++) step(hitbox);}
	inline void solveWorld() {
		for (size_t i = 0; i < wTime.count; i++) step_world();
		for (size_t i = 0; i < rTime.count; i++) world->weather.update(rTime.step);
	}

private:

	void step(Hitbox& hibox);
	void step_world();
	void spreadLiquid(Voxel src, Voxels& voxs, Chunk* chunk, const glm::ivec3& local);
	void modelingLuqid(size_t index, Voxels& voxs, Chunk* chunk);
	

	struct PhStep {
		PhStep():step(1/60.f) {}
		PhStep(float step_): step(step_){}
		inline void up(float time) {
			time = time + rem;
			count = (int)(time / step);
			rem = time - (float)count * step;
		}
		int count;
		float step;
	private:
		float rem = 0.f;
	};

	PhStep rTime;
	PhStep wTime;
	glm::vec3 gravity;

	GlobalScene*scene;
	World* world;
	size_t dist_mod = REGION_SIZE;
};
#endif
