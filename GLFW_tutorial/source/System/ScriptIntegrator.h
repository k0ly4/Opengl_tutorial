#ifndef SCRIPT_INTEGRATOR_H
#define SCRIPT_INTEGRATOR_H
#include "LuaContext.h"
class sScriptIntegrator
{
public:
	static void integrateGlobal() {
		integrate();
	}
private:
	static void voxConstants(luke::LuaInterface& script);
	static void generatorConstants(luke::LuaInterface script);
	static void integrate();
	
	sScriptIntegrator() {}
	~sScriptIntegrator() {}
};
#endif

