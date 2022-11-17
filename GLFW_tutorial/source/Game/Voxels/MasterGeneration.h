#ifndef MASTER_GENERATION
#define MASTER_GENERATION

#include "Chunk.h"
#include "FastNoiseLite.h"


/////qRandom----------------------------------
class qRandom {

public:

	qRandom() {
		seed = (unsigned)time(0);
	}

	inline int rand() {
		seed = (8253729 * seed + 2396403);
		return seed % 32768;
	}

	inline void setSeed(size_t number) {
		seed = number + 8253729;
		rand();
	}

private:

	size_t seed;

};


/////FastNoise----------------------------------
class FastNoise {

public:
	FastNoise() {
		//NoiseType_Perlin              =1358
		//NoiseType_Cellular            =3682 =3584 =3485
		//NoiseType_OpenSimplex2        =779
		//NoiseType_OpenSimplex2S       =928
		//NoiseType_Value				=1000 1283 =993
		//NoiseType_OpenSimplex2S       =5034  =5164
		setTypeNoise(FastNoiseLite::NoiseType_OpenSimplex2);
	}

	inline float get(const glm::vec3& pos) {
		return noise.GetNoise(pos.x,pos.y,pos.z);
	}
	inline float get(const glm::vec2& pos) {
		return noise.GetNoise(pos.x, pos.y);
	}

	inline float get(const glm::uvec2& pos) {
		return noise.GetNoise((float)pos.x, (float)pos.y);
	}
	inline float get(const glm::uvec3& pos) {
		return noise.GetNoise((float)pos.x, (float)pos.y, (float)pos.z);
	}
	
	
	inline void setSeed(int seed) {
		noise.SetSeed(seed);
	}
	inline void setTypeNoise(FastNoiseLite::NoiseType type) {
		noise.SetNoiseType(type);
	}
private:
	FastNoiseLite noise;
};


/////MasterGaneration----------------------------------
class MasterGeneration
{
public:
	static void generateChunk(Chunk& chunk);
private:

	static size_t calcHeight(const glm::uvec2& real);
	MasterGeneration() {}
	~MasterGeneration() {}

	static FastNoise noise;
};
#endif

