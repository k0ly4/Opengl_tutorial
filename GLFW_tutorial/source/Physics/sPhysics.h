#ifndef S_PHYSICS_H
#define S_PHYSICS_H

#include "PhysicsSolver.h"
#include "System/LuaContext.h"
class GlobalScene;

class PhysicsModule {
public:

	PhysicsModule() {}
	void init(GlobalScene& scene);
	void update(float time, GlobalScene& scene);

private:
	inline void initLuaScript() {
		LUA_TRY
		script.open("scripts\\i_physics.lua");
		luke::LuaRef phys = script.get("physics_config");
		core.gravity =			phys["c_gravity"];
		core.air_resistance =	phys["c_air_resistance"];
		LUA_CATCH
	}

	luke::LuaInterface script;
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

