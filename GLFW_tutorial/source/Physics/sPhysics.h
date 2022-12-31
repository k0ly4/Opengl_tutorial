#ifndef S_PHYSICS_H
#define S_PHYSICS_H

#include "PhysicsSolver.h"
class GlobalScene;

class PhysicsModule {
public:

	PhysicsModule() {}
	void init(GlobalScene& scene);
	void update(float time, GlobalScene& scene);

private:

	PhysicsSolver core;

};
class sPhysics
{
public:

private:

	sPhysics() {}
	~sPhysics() {}

};

#endif

