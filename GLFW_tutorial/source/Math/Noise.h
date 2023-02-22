#ifndef MATH_NOISE_H
#define MATH_NOISE_H

#include "FastNoiseLite.h"
#include "Math/GLM.h"
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

	inline float get(const glm::vec3& pos)noexcept { return noise.GetNoise(pos.x, pos.y, pos.z); }
	//[-1,1]
	inline float get(const glm::vec2& pos)noexcept { return noise.GetNoise(pos.x, pos.y); }
	//[0,1]
	inline float getNormalize(const glm::vec2& pos)noexcept { return (noise.GetNoise(pos.x, pos.y) + 1.f) * 0.5f; }
	inline float getNormalizeFactor(float x, float y, float factor)noexcept { return (noise.GetNoise(x * factor, y * factor) + 1.f) * 0.5f; }
	/// 2D noise at given position using current settings
	inline float get(const glm::uvec2& pos)noexcept { return noise.GetNoise((float)pos.x, (float)pos.y); }
	inline float get(const glm::uvec3& pos)noexcept { return noise.GetNoise((float)pos.x, (float)pos.y, (float)pos.z); }
	inline float get(float x, float y, float z)noexcept { return noise.GetNoise(x, y, z); }

	inline void setSeed(int seed) { noise.SetSeed(seed); }
	inline void setTypeNoise(FastNoiseLite::NoiseType type) { noise.SetNoiseType(type); }

private:
	FastNoiseLite noise;
};
#endif 