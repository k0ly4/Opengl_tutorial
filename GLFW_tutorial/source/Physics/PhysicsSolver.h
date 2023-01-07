#ifndef PHYSICS_SOLVER_H
#define PHYSICS_SOLVER_H

#include "Hitbox.h"
#include "Game/Voxels/World.h"
//PhysicsSolver----------------------------------------------
/// <summary>
/// ѕока что основной класс на который нужно жаловатьс€ за ошибки в физической имитации
/// </summary>
/// 
class  GlobalScene;
class PhysicsSolver
{
public:
	PhysicsSolver() :
		wTime(1 / 2.f),
		rTime(1 / 60.f)
	{}
	void init(GlobalScene* scene_);
	inline void upTime(float time) {

		float rtime = (time+rTime.rem);
		rPhStep = (int)(rtime / rTime.step);
		rTime.rem = rtime - (float)rPhStep* rTime.step;
		if (rPhStep > 5) LOG("rPhStep:%d\n", rPhStep);

		float wtime = (time + wTime.rem) ;
		wPhStep = (int)(wtime / wTime.step);
		wTime.rem = wtime - (float)wPhStep* wTime.step;
	}

	inline void setGravity(const glm::vec3& gravity_) { gravity = gravity_;}
	inline void solve(Hitbox& hitbox) {	for (size_t i = 0; i < rPhStep; i++) step(hitbox);}
	inline void solveWorld() {
		
		for (size_t i = 0; i < wPhStep; i++) step_world();
		for (size_t i = 0; i < rPhStep; i++) world->weather.update(rTime.step);
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

	int rPhStep;
	int wPhStep;
	UpTime rTime;
	UpTime wTime;

	GlobalScene*scene;
	World* world;
};
#endif
