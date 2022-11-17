#include "MasterGeneration.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>



inline float getNoise(const glm::vec3& global) {
	return glm::perlin(glm::vec3(global) * 0.0125f);
}

size_t  MasterGeneration::calcHeight(const glm::uvec2& real) {
	
	float height = noise.get(real);
	height = height * 18.f + 40.f;
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

void MasterGeneration::generateChunk(Chunk& chunk) {

	//for (size_t y = 0; y < CHUNK_H; y++) {
	//	for (size_t z = 0; z < CHUNK_D; z++) {
	//		for (size_t x = 0; x < CHUNK_W; x++)
	//		{
	//			glm::uvec3 real_coord = glm::uvec3(x, y, z) + chunk.getGlobalPos();
	//			int id = (noise.get(glm::vec3(real_coord) * 0.0125f) > 0.1f) - 1;//real_y <= (height) * 60 + 30;
	//			if (real_coord.y <= 2)
	//				id = 1;
	//			chunk.getFromLocalCoord(x, y, z).id = id;
	//		}
	//	}
	//}
	size_t heights[CHUNK_D*CHUNK_W];

	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {
			glm::uvec2 real = glm::uvec2(x+chunk.getGlobalPos().x,z + chunk.getGlobalPos().z) ;
			heights[z * CHUNK_W + x] = calcHeight(real);
		}
	}

	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {
			size_t height = heights[z * CHUNK_W + x];

			for (size_t y = 0; y < CHUNK_H; y++) {

				glm::uvec3 real = glm::uvec3(x, y, z) + chunk.getGlobalPos();
				
				int id = vox::air;
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
				/*else {
					int tree = generate_tree(&noise, &random, heights, real_x, real_y, real_z, 16);
					if (tree)
						id = tree;
					else if ((tree = generate_tree(&noise, &random, heights, real_x, real_y, real_z, 19))) {
						id = tree;
					}
					else if ((tree = generate_tree(&noise, &random, heights, real_x, real_y, real_z, 23))) {
						id = tree;
					}
				}*/
				chunk.getFromLocalCoord(x, y, z).id = id;
			}
		}
	}


	chunk.setModified();
	chunk.isGenerated = 1;
	chunk.isInitLightMap = 0;
}
FastNoise MasterGeneration::noise;