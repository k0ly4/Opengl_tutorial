#include "ScriptIntegrator.h"
#include "Physics/Hitbox.h"
#include "Game/Entities/GeneralGameEntity.h"
#include "Game/Voxels/MasterGeneration.h"
void sScriptIntegrator::voxConstants(luke::LuaInterface& script) {
	using namespace vox;
	script
		.beginNamespace("vox")
		.addConstant<twobyte>("air", air)
		.addConstant<twobyte>("turf", turf)
		.addConstant<twobyte>("earth", earth)
		.addConstant<twobyte>("sand", sand)
		.addConstant<twobyte>("snow", snow)
		.addConstant<twobyte>("stone", stone)
		.addConstant<twobyte>("glass", glass)
		.addConstant<twobyte>("glowstone", glowstone)
		.addConstant<twobyte>("bedrock", bedrock)
		.addConstant<twobyte>("lamp", lamp)
		.addConstant<twobyte>("oak", oak)
		.addConstant<twobyte>("grass", grass)
		.addConstant<twobyte>("beacon", beacon)
		.addConstant<twobyte>("redstone", redstone)
		.addConstant<twobyte>("water", water)
		.addConstant<twobyte>("size", size)
		.endNamespace();
}
void sScriptIntegrator::generatorConstants(luke::LuaInterface script) {
	script
		.beginNamespace("NoiseType")
		.addConstant<int>("OpenSimplex2",	static_cast<int>(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2))
		.addConstant<int>("OpenSimplex2S",	static_cast<int>(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2S))
		.addConstant<int>("Cellular",		static_cast<int>(FastNoiseLite::NoiseType::NoiseType_Cellular))
		.addConstant<int>("Perlin",			static_cast<int>(FastNoiseLite::NoiseType::NoiseType_Perlin))
		.addConstant<int>("ValueCubic",		static_cast<int>(FastNoiseLite::NoiseType::NoiseType_ValueCubic))
		.endNamespace();
}
void sScriptIntegrator::integrate() {
	LUA_TRY
	LOG("Lua integration-");
	using namespace luke;
	using namespace vox;
	LuaInterface script;


	//constants----------------------------
	generatorConstants(script);
	voxConstants(script);
	//glm::vec2----------------------------
	script.global()
		.beginClass <glm::vec2 >("vec2")
		.addConstructor<void(*)(void)>()
		.addConstructor<void(*)(float, float)>()
		.addProperty("x", &glm::vec2::x)
		.addProperty("y", &glm::vec2::y)
		.endClass();
	//glm::vec3----------------------------
	script.global()
		.beginClass <glm::vec3 >("vec3")
		.addConstructor<void(*)(void)>()
		.addConstructor<void(*)(float, float, float)>()
		.addProperty("x", &glm::vec3::x)
		.addProperty("y", &glm::vec3::y)
		.addProperty("z", &glm::vec3::z)
		.endClass();
	//Hitbox -----------------------------
	script.global()
		.beginClass<Hitbox>("Hitbox")
		.addConstructor<void(*)(void)>()
		.addProperty("pos",			&Hitbox::position)
		.addProperty("hSize",		&Hitbox::halfsize)
		.addProperty("vel",			&Hitbox::velocity)
		.addProperty("mass",		&Hitbox::mass)
		.addProperty("grounded",	&Hitbox::grounded)
		.endClass();
	
	//map----------------------------
	script.
		beginNamespace("map")
		.addFunction("addChannelHeight", &CustomGenerator::addChannelHeight)
		.endNamespace();
	//FastNoise----------------------------
	script.global()
		.beginClass<FastNoise>("FastNoise")
		.addConstructor<void(*)(void)>()
		.addFunction("getNormalize", &FastNoise::getNormalizeFactor)
		.endClass();
	//Range<byte>----------------------------
	script.global()
		.beginClass<Range<byte>>("Range")
		.addConstructor<void(*)(void)>()
		.addConstructor<void(*)(byte, byte)>()
		.addFunction("is", &Range<byte>::is)
		.addProperty("min", &Range<byte>::min)
		.addProperty("max", &Range<byte>::max)
		.endClass();
	//Biom----------------------------
	script.global()
		.beginClass<Biom>("Biom")
		.addConstructor<void(*)(void)>()
		.addProperty("r_tmpr", &Biom::r_tmpr)
		.addProperty("r_humidity", &Biom::r_humidity)
		.addProperty("ground", &Biom::ground)
		.addProperty("underground", &Biom::underground)
		.endClass();
	LOG("success\n");
		LUA_CATCH

}