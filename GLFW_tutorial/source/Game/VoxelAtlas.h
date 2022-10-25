#ifndef VOXEL_ATLAS_H
#define VOXEL_ATLAS_H

#include "Math/Math.h"
#include "Graphic/Texture.h"
#include "System/Exception.h"
#include "Resource/Shader.h"

enum Side :size_t
{
	top,
	bottom,
	right,
	left,
	front,
	back
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

///VoxelType----------------------------------------------
/// <summary>
/// 
/// </summary>
struct UvVoxel {

	void setSolid(int fill) {
		for (size_t i = 0; i < 6; i++)
			id[i] = fill;
	}
	int id[6];
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
		uv_max,
	};

	enum IdVoxel :int
	{
		id_air = -1,
		id_turf = 0,
		id_earth,
		id_max,
	};


	void load(const std::string& path, size_t sizeVoxel);

	inline const glm::vec2& get(int id, int side)const {
		return uv[scan[id].id[side]];
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
	std::vector<UvVoxel> scan;

	float uvSize_;
	size_t sizeVoxel_ = 32;
	Texture2D texture_;

};
#endif