#ifndef S_PHYSICS_H
#define S_PHYSICS_H

#include "PhysicsSolver.h"
class Scene;

class PhysicsModule {
public:

	PhysicsModule() {}
	void init(Scene& scene);
	void update(float time, Scene& scene);

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

