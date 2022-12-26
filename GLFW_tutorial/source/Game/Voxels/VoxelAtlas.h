#ifndef VOXEL_ATLAS_H
#define VOXEL_ATLAS_H

#include "Math/Math.h"
#include "Graphic/Texture.h"
#include "System/Exception.h"
#include "Resource/Shader.h"
namespace Side2D {

	enum eSide2D :byte
	{
		left,
		right,
		bottom,
		top,
		//corner
		left_bottom,
		right_bottom,
		left_top,
		right_top,

		NuN,
	};
	inline eSide2D opposite(eSide2D side) {
		if (side == right)return left;
		if (side == left)return right;
		if (side == top)return bottom;
		return top;
	}
	inline eSide2D getCornerNorm(bool x,bool y) {
		if (x) return	y ? right_top : right_bottom;
		else return		y ? left_top : left_bottom;
	}

	inline eSide2D getMainSide(int x, int y) {
		if (x > 0)return right;
		if (x < 0)return left;
		if (y < 0)return bottom;
		return top;
	}
	//normalize from [0;maxX] to [-1;1] 
	inline int normalizeU(size_t x, size_t maxX) {
		return x == maxX ? 1 : x == 0 ? -1 : 0;
	}
	//normalize from [-infinity; +infinity] to [-1;1]
	inline int normalizeI(int coord_x, int maxX) {
		return (coord_x > maxX) ? 1 : (coord_x < 0) ? -1 : 0;
	}

	inline eSide2D toSide(int norm_x, int norm_y) {
			if ((bool)norm_x == (bool)norm_y) return getCornerNorm((norm_x + 1), (norm_y + 1));
			return getMainSide(norm_x, norm_y);
	}

	inline eSide2D toSideU(size_t x, size_t y, size_t max) {	return toSide(normalizeU(x, max), normalizeU(y, max));}

	inline eSide2D toSideI(int x, int y, size_t max)		{	return toSide(normalizeI(x, max), normalizeI(y, max));}
}
namespace Side {
	enum eSide:size_t
	{
		left,
		right,
		bottom,
		top,
		back,
		front,
	};

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
		return blocks[id.e.id];
	}
	//path to json file
	bool load(const std::string& directory);

	inline const glm::vec2& get(short id, int side)const {
		return uv[blocks[id].idSide[side]];
	}

	inline const glm::vec2& get(Voxel id, int side)const {
		return get(id.e.id, side);
	}

	inline float getNormalizeSizeVoxel()const {return uvSize_;}
	inline size_t getSizeVoxel()const { return sizeVoxel_; }

	void use(const Shader& shader)const {
		shader.uniform("configMaterial", 1);
		shader.uniform("baseColor", glm::vec3(1.f));
		texture_.use(0);
	}
	const Texture2D& getTexture()const {return texture_;}
private:

	inline glm::vec2 getUV(int id) {
		float u = (id % voxelInSide) * uvSize_;
		float v = 1.f - ((1.f + id / voxelInSide) * uvSize_);
		return glm::vec2(u, v);
	}

	std::vector<glm::vec2> uv;
	std::vector<Block> blocks;

	float uvSize_;
	size_t voxelInSide;
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
		return (id.e.id > vox::air) && (get(id).emissionFlag == 1);
	}
	static inline bool isRender(Voxel id) {
		return id.e.id > vox::air;
	}
	static inline bool isSelectable(Voxel voxel) {
		return (voxel.e.id != vox::air);
	}
	static inline bool isOpaque(Voxel id) {
		return isRender(id) && (get(id).drawGroup == 0);
	}
	static inline bool isObstacle(Voxel id) {
		if (id.e.id < 0)return 0;
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