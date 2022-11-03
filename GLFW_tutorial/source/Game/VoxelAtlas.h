#ifndef VOXEL_ATLAS_H
#define VOXEL_ATLAS_H

#include "Math/Math.h"
#include "Graphic/Texture.h"
#include "System/Exception.h"
#include "Resource/Shader.h"

enum Side :size_t
{
	left,
	right,
	bottom,
	top,
	back,
	front,
};
inline Side getSide(const glm::ivec3& dist) {
	Side side;
	if (dist.x == -1) side = left;
	else if (dist.x == 1) side = right;
	else if (dist.y == -1) side = bottom;
	else if (dist.y == 1) side = top;
	else if (dist.z == -1) side = back;
	else if (dist.z == 1) side = front;
	else LOG(LogError, "Exit setcloses::side not init\n");
	return side;
}

inline Side getSide(int x, int y, int z) {
	return getSide(glm::ivec3(x, y, z));
}

inline int normalizeSign(int x, int max) {
	return x >= max ? 1 : x < 0 ? -1 : 0;
}

inline Side	getSide(int x, int y, int z, const glm::uvec3& max) {
	return getSide(normalizeSign(x,max.x),normalizeSign(y,max.y),normalizeSign(z,max.z));
}
inline Side	getSide(int x, int y, int z, const glm::ivec3& max) {
	return getSide(normalizeSign(x, max.x), normalizeSign(y, max.y), normalizeSign(z, max.z));
}
///VoxelType----------------------------------------------
/// <summary>
/// 
/// </summary>
struct Block {

	void setSolid(int fill) {
		for (size_t i = 0; i < 6; i++)
			idSide[i] = fill;
	}
	int idSide[6];
};

///VoxelAtlas---------------------------
/// <summary>
/// 
/// </summary>
class VoxelAtlas {

public:

	enum IdUV :int
	{
		uv_turf = 0,
		uv_side_turf,
		uv_earth,
		uv_light,
		uv_max,
	};

	enum IdVoxel :int
	{
		id_air = -1,
		id_turf = 0,
		id_earth,
		id_light,
		id_max,
	};


	void load(const std::string& path, size_t sizeVoxel);
	//path to json file
	bool load(const std::string& directory);

	inline const glm::vec2& get(int id, int side)const {
		return uv[blocks[id].idSide[side]];
	}

	inline const glm::vec2& get(const Voxel& id, int side)const {
		return get(id.id, side);
	}

	inline float getVoxelSize()const {
		return uvSize_;
	}

	void use(const Shader& shader)const {
		shader.uniform("configMaterial", 1);
		shader.uniform("baseColor", glm::vec3(1.f));
		texture_.use(0);
	}

private:

	glm::vec2 getUV(int id) {
		float u = (id % sizeVoxel_) * uvSize_;
		float v = 1.f - ((1.f + id / sizeVoxel_) * uvSize_);
		return glm::vec2(u, v);
	}

	std::vector<glm::vec2> uv;
	std::vector<Block> blocks;

	float uvSize_;
	size_t sizeVoxel_;
	Texture2D texture_;

};
#endif