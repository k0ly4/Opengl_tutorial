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
		if (side == right)	return left;
		if (side == left)	return right;
		if (side == top)	return bottom;
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
	//uv wrap
	int idSide[6];
	//emission
	Uint8RGB emission;
	//Draw method
	byte drawGroup;
	//physiccs
	byte physGroup;
	//is emission
	bool isEmission;

	inline void setSolid(int fill) { for (size_t i = 0; i < 6; i++) idSide[i] = fill;}
	inline void setEmissionFlag() {isEmission = emission.r || emission.g || emission.b;}
};

///VoxelAtlas---------------------------
/// <summary>
/// 
/// </summary>
class ResourcePack {

public:
	enum eDrawGroup:byte {
		draw_non =0,
		draw_opaque,
		draw_transparent,
		draw_alpha,
	};
	enum ePhysics :byte {
		ph_gas = 0,
		ph_solid,
		ph_liquid,
	};

	const Block& get(Voxel id)const noexcept { return blocks[id.e.id];}
	//path to json file
	bool load(const std::string& directory);
	inline const glm::vec2& get(Voxel id, int side)const noexcept { return uv[blocks[id.e.id].idSide[side]];}

	inline float getNormalizeSizeVoxel()const noexcept {return uvSize_;}
	inline size_t getSizeVoxel()const noexcept { return sizeVoxel_; }

	inline void use(const Shader& shader)const {
		shader.uniform("configMaterial", 1);
		shader.uniform("baseColor", glm::vec3(1.f));
		texture_.use(0);
	}
	const Texture2D& getTexture()const {return texture_;}
private:

	inline const glm::vec2& get(twobyte id, int side)const { return uv[blocks[id].idSide[side]];}
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
class VoxPack {
public:
	
	static inline const Block& get(Voxel id)				noexcept { return res->get(id);}
	static inline const glm::vec2& get(Voxel id, byte side)	noexcept { return res->get(id, side);}
	static inline const ResourcePack* get()					noexcept { return res; }
	//Light parr
	static inline bool isEmission(Voxel id)					noexcept { return  get(id).isEmission;}
	//Draw parr
	static inline bool isRender(Voxel id)					noexcept { return (get(id).drawGroup != ResourcePack::draw_non); }
	static inline bool isSelectable(Voxel id)				noexcept { return (get(id).drawGroup != ResourcePack::draw_non); }
	static inline bool isOpaque(Voxel id)					noexcept { return (get(id).drawGroup == ResourcePack::draw_opaque);}
	static inline bool isAlpha(Voxel id)					noexcept { return (get(id).drawGroup == ResourcePack::draw_alpha); }
	//Physics parr
	static inline bool isGas(Voxel id)						noexcept { return get(id).physGroup == ResourcePack::ph_gas; }
	static inline bool isSolid(Voxel id)					noexcept { return get(id).physGroup == ResourcePack::ph_solid;}
	static inline bool isLiquid(Voxel id)					noexcept { return get(id).physGroup == ResourcePack::ph_liquid; }
	static inline bool isSourceLiquid(Voxel id)				noexcept { return id.e.m1 == maxConcLiquid; }
	//пока что только если жидкость
	static inline bool isActive(Voxel id)					noexcept { return get(id).physGroup == ResourcePack::ph_liquid; }

	static inline void set(const ResourcePack* resource)	noexcept { res = resource;}
	const static byte maxConcLiquid = 0x07u;
private:

	VoxPack() {}
	~VoxPack() {}
	static const ResourcePack* res;

};
#endif