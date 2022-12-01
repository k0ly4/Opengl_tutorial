#ifndef VOXEL_ATLAS_H
#define VOXEL_ATLAS_H

#include "Math/Math.h"
#include "Graphic/Texture.h"
#include "System/Exception.h"
#include "Resource/Shader.h"

enum Side :byte
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

	std::string name;
	int idSide[6];
	Uint8RGB emission;
	byte drawGroup;
	byte physGroup;

	bool emissionFlag;

	inline void setSolid(int fill) {
		for (size_t i = 0; i < 6; i++)
			idSide[i] = fill;
	}

	inline void setEmissionFlag() {
			emissionFlag = emission.r || emission.g || emission.b;
	}
};

///VoxelAtlas---------------------------
/// <summary>
/// 
/// </summary>
class ResourceVoxelPack {

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

	const Block& get(Voxel id)const  {
		return blocks[id.id];
	}

	
	//path to json file
	bool load(const std::string& directory);

	inline const glm::vec2& get(int id, int side)const {
		return uv[blocks[id].idSide[side]];
	}

	inline const glm::vec2& get(Voxel id, int side)const {
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

	inline glm::vec2 getUV(int id) {
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

///VoxelPack---------------------------
/// <summary>
/// 
/// </summary>
class VoxelPack {
public:

	static inline const Block& get(Voxel id) {
		return pack->get(id);
	}
	static inline const glm::vec2& get(Voxel id, byte side) {
		return pack->get(id, side);
	}
	static inline const ResourceVoxelPack* get() {
		return pack;
	}

	static inline bool isEmission(Voxel id) {
		return (id.id > -1) && (get(id).emissionFlag == 1);
	}
	static inline bool isRender(Voxel id) {
		return id.id > -1;
	}
	static inline bool isSelectable(Voxel voxel) {
		return (voxel.id != -1);
	}
	static inline bool isOpaque(Voxel id) {
		return isRender(id) && (get(id).drawGroup == 0);
	}
	static inline bool isObstacle(Voxel id) {
		if (id.id < 0)return 0;
		return get(id).physGroup == 1;
	}

	static inline void set(const ResourceVoxelPack* pack_){
		pack = pack_;
	}

private:

	VoxelPack() {}
	~VoxelPack() {}
	static const ResourceVoxelPack* pack;

};
#endif