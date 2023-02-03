#include "MasterGeneration.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>

qRandom sQrand::random;

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

void CustomGenerator::initScript() {
	LUA_TRY
		luke::LuaRef generator =	script.get("generator_config");
		initBioms(generator);
		luke::LuaRef h_map = generator["heights_map"];

		m_heights.f_scale =		h_map["scaleFactor"];
		m_heights.f_sub =		h_map["subFactor"];
		m_heights.noise.setTypeNoise(static_cast<FastNoiseLite::NoiseType>(h_map["noiseType"].cast<int>()));

		h_map["init"]();

	
	LUA_CATCH
}

void CustomGenerator::initBioms(luke::LuaRef generator) {
	using namespace vox;
	LUA_TRY

		luke::LuaRef biom_map = generator["biom_map"];
	
		m_biom.f_humidiy =	biom_map["scaleHumidity"];
		m_biom.f_tmpr =		biom_map["scaleTemperature"];
		m_biom.noise.setTypeNoise(static_cast<FastNoiseLite::NoiseType>(biom_map["noiseType"].cast<int>()));
		
		luke::LuaRef lua_biom = biom_map["bioms"];
		lua_biom["getPlane"](&bioms[Biom::plain]);
		lua_biom["getDesert"](&bioms[Biom::desert]);
		lua_biom["getTundra"](&bioms[Biom::tundra]);

		//LOG("Okay");
	LUA_CATCH
}

size_t  CustomGenerator::calcHeight(const glm::vec2& real) {
	float height = 0.f;

	for (size_t i = 0; i < pars.size(); i++) { height += m_heights.noise.getNormalize(real * pars[i].scale) * pars[i].influence; }

	height = height * m_heights.f_scale + m_heights.f_sub;
	return height;
}

Voxel blockBiom(const glm::uvec3& real, size_t height, vox::Biom biom) {
	if (real.y == height) return biom.ground;

	if (real.y < height) {
		if (real.y == 0) return vox::bedrock;
		if (real.y < height - 6) return vox::stone;
		return biom.underground;
	}
	return vox::air;
}

void CustomGenerator::generate(Chunk& chunk) {
	m_green.clear();
	Voxels& voxels = chunk.voxels();
	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {

			size_t index = toInt(x, z, CHUNK_D);

			glm::vec2 real = glm::vec2(x + chunk.voxelPos().x, z + chunk.voxelPos().z);
			//height
			m_heights.map[index] = calcHeight(real);
			//biom
			m_biom.map[index] = getBiom(real.x, real.y);
			//greenary
			if (m_green.map[index] == 0) {
				m_green.map[index] =std::min(((std::rand() % 10)+1), 2);
				if (m_green.map[index] == 1) m_green.fillCircle(index);
			}

		}
	}
	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {

			size_t height = m_heights.map(x,z);
			glm::uvec3 global = glm::uvec3(x + chunk.voxelPos().x,0, z + chunk.voxelPos().z);

			for (size_t y = 0; y < CHUNK_H; y++) {

				glm::uvec3 local(x, y, z);
				global.y = y + chunk.voxelPos().y;
				Voxel id = vox::air;
				id = blockBiom(global, height, bioms[m_biom.map(x, z)]);
				voxels(local) = Voxel(id);
				
			}
			if (m_green.map(x, z) == 1) initTree(voxels, glm::ivec3(x, height + 1, z));
		}
	}

	chunk.flag.generate();
}
std::vector< CustomGenerator::Par>CustomGenerator::pars;