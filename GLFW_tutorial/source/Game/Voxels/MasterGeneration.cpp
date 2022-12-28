#include "MasterGeneration.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>

Generator* MasterGeneration::generator = 0;

inline float getNoise(const glm::vec3& global) {
	return glm::perlin(glm::vec3(global) * 0.0125f);
}

size_t  DefaultGenerator::calcHeight(const glm::vec2& real) {
	
	float height = 0.f;
	height += noise.getNormalize(real * 0.1f);
	height += noise.getNormalize(real * 0.5f);
	height += noise.getNormalize(real * 0.7f);
	height += noise.getNormalize(real);
	height *= noise.getNormalize(real		*2.f);
	height = height * 12.f + 32.f;
	return height;
	//const float s = 0.2f;
	/*float height = fnlGetNoise3D(noise, real_x * 0.0125f * s * 32, real_z * 0.0125f * s * 32, 0.0f);
	height += fnlGetNoise3D(noise, real_x * 0.025f * s * 32, real_z * 0.025f * s * 32, 0.0f) * 0.5f;
	height += fnlGetNoise3D(noise, real_x * 0.05f * s * 32, real_z * 0.05f * s * 32, 0.0f) * 0.25f;
	height += fnlGetNoise3D(noise, real_x * 0.1f * s * 32, real_z * 0.1f * s * 32, 0.0f) * 0.225f;
	height += fnlGetNoise3D(noise, real_x * 0.2f * s * 32, real_z * 0.2f * s * 32, 0.0f) * 0.125f;
	height += fnlGetNoise3D(noise, real_x * 0.4f * s * 32, real_z * 0.4f * s * 32, 0.0f) * 0.125f * 0.5F;
	height = height * 0.5f + 0.5f;
	height *= height;
	height *= (140.0f) * 0.12f / s;
	height += (42) * 0.12f / s;*/
	//return height;
}

void DefaultGenerator::generate(Chunk& chunk) {

	size_t heights[CHUNK_D*CHUNK_W];

	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {
			glm::vec2 real = glm::vec2(x+chunk.voxelPos().x,z + chunk.voxelPos().z) ;
			heights[z * CHUNK_W + x] = calcHeight(real);
		}
	}

	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {
			size_t height = heights[z * CHUNK_W + x];

			for (size_t y = 0; y < CHUNK_H; y++) {

				glm::uvec3 real = glm::uvec3(x, y, z) + chunk.voxelPos();
				
				short id = vox::air;
				if (real.y == height) {
					id = vox::turf;
				}
				else if (real.y < height) {
					if (real.y < height - 6) {
						id = vox::stone;
					}
					else {
						id = vox::earth;
					}
						
				}
				chunk.voxels()(x, y, z) = Voxel(id);
			}
		}
	}
	chunk.flag.generate();
}
void setConstants(luascr::LuaInterface& script) {
	script
		.beginNamespace("vox")
		.addConstant("air", -1)
		.addConstant("turf", 0)
		.addConstant("earth", 1)
		.addConstant("stone", 2)
		.addConstant("bedrock", 6)
		.addConstant("oak", 8)
		.addConstant("oak_wood", 9)
		.addConstant("snow", 10)
		.addConstant("sand", 11)
		.endNamespace();
}

void CustomGenerator::integrateScript() {
	try
	{
		setConstants(script);
		
		script.
			beginNamespace("map")
			.addFunction("addChannelHeight", &CustomGenerator::addChannelHeight)
			.endNamespace();

		script.global()
			.beginClass<FastNoise>("FastNoise")
			.addConstructor<void(*)(void)>()
			.addFunction("getNormalize", &FastNoise::getNormalizeFactor)
			.endClass();

		}
	catch (const std::exception& e)
	{
		LOG("%s\n", e.what());
		abort();
	}
}

void CustomGenerator::initScript() {
	
	try
	{
		luascr::LuaRef table = script.get("generator");
		multFactorHeight = table["multHeightFactor"];
		subFactorHeight =  table["subHeightFactor"];
		table["init"]();
	}
	catch (const std::exception&e)
	{
		LOG("%s\n", e.what());
		abort();
	}
}

size_t  CustomGenerator::calcHeight(const glm::vec2& real) {
	float height = 0.f;

	for (size_t i = 0; i < pars.size(); i++) {height += noise.getNormalize(real * pars[i].scale) * pars[i].influence; }

	height = height * multFactorHeight + subFactorHeight;
	return height;
}

int blockPlain(const glm::uvec3& real, size_t height) {
	if (real.y == height) return vox::turf;
	
	if (real.y < height) {
		if (real.y == 0) return vox::bedrock;
		if (real.y < height - 6) return vox::stone;
		return vox::earth;
	}
	return vox::air;
}

int blockDesert(const glm::uvec3& real, size_t height) {
	if (real.y == height) return vox::sand;

	if (real.y < height) {
		if (real.y == 0) return vox::bedrock;
		if (real.y < height - 6) return vox::stone;
		return vox::sand;
	}
	return vox::air;
}

int blockTundra(const glm::uvec3& real, size_t height) {
	if (real.y == height) return vox::snow;

	if (real.y < height) {
		if (real.y == 0) return vox::bedrock;
		if (real.y < height - 6) return vox::stone;
		return vox::earth;
	}
	return vox::air;
}
void CustomGenerator::generate(Chunk& chunk) {

	size_t heights[CHUNK_D * CHUNK_W];

	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {
			glm::vec2 real = glm::vec2(x + chunk.voxelPos().x, z + chunk.voxelPos().z);
			heights[z * CHUNK_W + x] = calcHeight(real);
		}
	}

	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {
			size_t height = heights[z * CHUNK_W + x];
			Biom::Type biom = getBiom(x+ chunk.voxelPos().x, z+ chunk.voxelPos().z);
			for (size_t y = 0; y < CHUNK_H; y++) {

				glm::uvec3 real = glm::uvec3(x, y, z) + chunk.voxelPos();
				short id = 55;
				if (biom == Biom::plain)
					id = blockPlain(real, height);
				else if (biom == Biom::tundra)
					id = blockTundra(real, height);
				else if (biom == Biom::desert)
					id = blockDesert(real, height);
				chunk.voxels()(x, y, z) = Voxel(id);
			}
		}
	}

	chunk.flag.generate();
}
std::vector< CustomGenerator::Par>CustomGenerator::pars;