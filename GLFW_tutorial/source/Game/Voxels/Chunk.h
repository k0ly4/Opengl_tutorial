#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H
#include "Graphic/Geometry.h"
#include "Game/Voxels/VoxelAtlas.h"
#include "Game/Light/LightMap.h"
#include "Scene/Convex.h"
#include <set>
///LightFace---------------------------------------------
//#define LightFace Vec4Array[4]
///Voxels---------------------------------------------
typedef Array3d<CHUNK_W, CHUNK_H, CHUNK_D, Voxel> Voxels;
///StateChunk---------------------------------------------
struct StateChunk {

public:

	inline bool isInitLight()const noexcept {		return is_init_light; }
	inline bool isGenerated()const noexcept {		return generated; }
	inline bool isModified()const	noexcept {		return modified; }
	inline bool isModifiedAlpha()const	noexcept {	return modifiedAlpha; }
	/// modified = 1; 
	inline void modify()noexcept { modified = 1; }
	/*modify();
	generated = 1;
	is_init_light = 0;*/
	inline void generate()noexcept {
		is_init_light = 0;
		generated = 1;
	}
	//generated = 0
	inline void offInit()noexcept { generated = 0;}
	/*modify();
	is_init_light = 1;*/
	inline void initLight()noexcept {
		modify();
		is_init_light = 1;
	}
	/// modifiedAlpha = 1; 
	inline void modifyAlpha() { modifiedAlpha=1; }
private:

	friend class gChunk;
	friend class MeshChunk;
	friend class Chunk;
	bool modifiedAlpha = 0;
	bool is_init_light = 0;
	bool generated = 0;
	bool modified = 1;

};

///gChunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class gChunk{
public:
	///Closes---------------------------------------------
	class Closes {

	public:

		Closes() {clear();}

		inline static void link(gChunk& lch, Side2D::eSide2D lside, gChunk& rch, Side2D::eSide2D rside) 
		{
			lch.closes.chunks[lside] = &rch; lch.flag.modify();
			rch.closes.chunks[rside] = &lch; rch.flag.modify();
		}

		inline void clear() { for (size_t i = 0; i < Side2D::NuN; i++) chunks[i] = nullptr;}

		inline void modify(size_t index)		{ if (chunks[index]) chunks[index]->flag.modify(); }
		inline void modifyAlpha(size_t index)	{ if (chunks[index]) chunks[index]->flag.modifyAlpha(); }

		inline void modify() { for (size_t i = 0; i < Side2D::NuN; i++) modify(i);}
		inline gChunk*& operator [](size_t i) noexcept { return chunks[i]; }
		inline void modify(size_t x, size_t y) {
			bool right =	(x == CHUNK_W - 1);
			bool left =		(x == 0);
			bool top =		(y == CHUNK_D - 1);
			bool bottom =	(y == 0);

			if(right)modify(Side2D::right);
			else if (left)modify(Side2D::left);

			if (top)modify(Side2D::top);
			else if (bottom) modify(Side2D::bottom);

			if (		top &&	right) modify(Side2D::right_top);
			else if (	top &&	left)modify(Side2D::left_top);
			else if (	bottom && right)modify(Side2D::right_bottom);
			else if (	bottom && left)modify(Side2D::left_bottom);
		}
		inline void modifyAlpha(size_t x, size_t y) {
			bool right = (x == CHUNK_W - 1);
			bool left = (x == 0);
			bool top = (y == CHUNK_D - 1);
			bool bottom = (y == 0);

			if (right)					modifyAlpha(Side2D::right);
			else if (left)				modifyAlpha(Side2D::left);

			if (top)					modifyAlpha(Side2D::top);
			else if (bottom)			modifyAlpha(Side2D::bottom);

			if (top && right)			modify(Side2D::right_top);
			else if (top && left)		modifyAlpha(Side2D::left_top);
			else if (bottom && right)	modifyAlpha(Side2D::right_bottom);
			else if (bottom && left)	modifyAlpha(Side2D::left_bottom);
		}
	private:
		gChunk* chunks[Side2D::NuN];
	};
	///SortableVoxel---------------------------------------------
	struct SortableVoxel {
		size_t ind;
		size_t d;
		SortableVoxel(size_t ind_) :ind(ind_){}
		SortableVoxel() {}
	};
	///gChunk---------------------------------------------
	gChunk(): bg_vox(0){}

	inline void init(size_t x,size_t z) {
		bg_ch = glm::uvec3(x, 0, z);
		bg_vox = bg_ch * CHUNK_VEC;
		flag.generated = 0;
	}
	//Local
	inline bool isUnVisible(size_t x, size_t y, size_t z)const { return !(!voxs.is(x, y, z) || VoxPack::isRender(voxs(x, y, z)));}
	//Global
	inline const Voxel* getGlobal(const glm::uvec3& coord) const{return voxs.get(coord - bg_vox);}
	inline Voxel* getGlobal(const glm::uvec3& coord) {return voxs.get(coord - bg_vox);}

	inline const glm::uvec3& voxelPos()const { return bg_vox; }
	inline const glm::uvec3& chunkPos()const { return bg_ch; }
	/// <summary>
	/// Изменяет заданный в глобальных кординатах coord воксель, устанавливая себе и 6 ближайшим чанкам modified в true
	/// </summary>
	void setVoxelLocal(Voxel vx, const glm::uvec3& coord);
	bool setVoxel(Voxel vx, glm::uvec3 coord) {
		coord = coord - bg_vox;
		if (voxs.is(coord)) {
			setVoxelLocal(vx, coord);
			return 1;
		}
		return 0;
	}

	inline byte getLight(const glm::uvec3& local, int channel_)		const{	return voxs.is(local) ? lightMap.get(local, channel_) : 0;}
	inline byte getLight(size_t x,size_t y,size_t z, int channel_)	const { return getLight(glm::uvec3(x, y, z), channel_);}
	inline byte getLightGlobal(const glm::uvec3& coord, int channel_)const {return lightMap.get(coord - bg_vox,channel_);}

	inline void setLightGlobal(const LightUint8& light, int channel_) {
		flag.modify();
		lightMap.set(light.pos - bg_vox, channel_, light.light);
	}

	Voxels& voxels()				{ return voxs;}
	const Voxels& voxels()const		{ return voxs;}
	
	Closes closes;
	LightMap lightMap;
	StateChunk flag;

	std::set<size_t> nonStatic;

protected:
	glm::ivec3 posView_;
	std::vector<SortableVoxel> s_vox;
	inline void upSortVox() {
		s_vox.clear();
		for (size_t i = 0; i < voxs.size(); i++) if (VoxPack::isAlpha(voxs[i])) s_vox.push_back(i);
	}
	static inline bool isFree(Voxel targ, byte drawGroup) { return !VoxPack::isRender(targ) || (VoxPack::get(targ).drawGroup != drawGroup);}
	inline bool isFree(int x, int y, int z, byte drawGroup)noexcept {
		///Local test
		if (voxs.is(x, y, z)) return isFree(voxs(x, y, z),drawGroup);
		//Global
		if ((size_t)y >= CHUNK_H) return 1;
		const gChunk* chunk = closes[Side2D::toSideI(x , z, CHUNK_W - 1)];
		if (chunk == nullptr) return 0;
		return isFree( chunk->voxels().clip(x,y,z), drawGroup);
	}
	//render
	glm::uvec3 bg_ch, bg_vox;
	Voxels voxs;
};

///MeshChunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class MeshChunk :public gChunk {
public:
	///ShellGeometry---------------------------------------------
	struct ShellGeometry :public iGeometry<VoxelVertex> {bool needUpBuffer = 0; };
	///MeshChunk---------------------------------------------
	MeshChunk() :
		gChunk(),
		posView_(0)
	{
		mesh_.VBO.setMode(GBO::DYNAMIC);
		mesh_.EBO.setMode(GBO::DYNAMIC);
		mesh_sort.VBO.setMode(GBO::DYNAMIC);
		mesh_sort.EBO.setMode(GBO::DYNAMIC);
	}
	//render
	void drawOpaqueMesh(const Shader& shader);
	void drawSortMesh(const Shader& shader);
	//message for need modify sort mesh

	ShellGeometry mesh_;
	ShellGeometry mesh_sort;

protected:
	//sync
	bool dBlock = 0, cBlock = 0;

	glm::ivec3 posView_;

	inline byte LIGHT(int x, int y, int z, int channel) {

		if (voxs.is(x, y, z)) return lightMap.get(x, y, z, channel);
		if ((size_t)y >= CHUNK_H) return 0;
		gChunk* chunk = closes[Side2D::toSideI(x, z, CHUNK_W - 1)];
		return chunk != 0 ? chunk->lightMap.get(::clip(x, CHUNK_W), ::clip(y, CHUNK_H), ::clip(z, CHUNK_D), channel) : 0;
	}
	inline ColorU LIGHT(int x, int y, int z) {
		if (voxs.is(x, y, z)) return lightMap.get(x, y, z);
		if ((size_t)y >= CHUNK_H) return ColorU(0, 0, 0, 0);
		gChunk* chunk = closes[Side2D::toSideI(x, z, CHUNK_W - 1)];
		return chunk != 0 ? chunk->lightMap.get(::clip(x, CHUNK_W), ::clip(y, CHUNK_H), ::clip(z, CHUNK_D)) : ColorU(0, 0, 0, 0);
	}
	inline Voxel VOXEL(int x, int y, int z)noexcept {
		///Local test
		if (voxs.is(x, y, z)) return voxs(x, y, z);
		//Global
		if ((size_t)y >= CHUNK_H) return 0;
		const gChunk* chunk = closes[Side2D::toSideI(x, z, CHUNK_W - 1)];
		return chunk != 0? chunk->voxels().clip(x, y, z):0;
	}

	inline byte LIGHT(const glm::ivec3& coord, int channel) { LIGHT(coord.x, coord.y, coord.z, channel); }

	inline glm::vec4 getChannels(int x, int y, int z) {
		return {(float)LIGHT(x,y,z,0), (float)LIGHT(x,y,z,1), (float)LIGHT(x,y,z,2), (float)LIGHT(x,y,z,3) };
	}
	void lightFaceXZ(	int x, int y, int z,	Vec4Array* face);
	void lightFaceYZ(	int x, int y, int z,	Vec4Array* face);
	void lightFaceXY(	int x, int y, int z,	Vec4Array* face);

	void buildTopFace(		ShellGeometry& mesh, int x, int y, int z, Voxel voxel, float uvsize);
	inline void buildTopFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
		if (isFree(x, y + 1, z, drawGroup))buildTopFace(mesh, x, y, z, voxel, uvsize);
	}
	void buildBottomFace(	ShellGeometry& mesh, int x, int y, int z, Voxel voxel, float uvsize);
	inline void buildBottomFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
		if (isFree(x, y - 1, z, drawGroup)) buildBottomFace(mesh, x, y, z, voxel, uvsize);
	}
	void buildRightFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel, float uvsize);
	inline void buildRightFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
		if (isFree(x + 1, y, z, drawGroup))buildRightFace(mesh, x, y, z, voxel, uvsize);
	}
	void buildLeftFace(		ShellGeometry& mesh, int x, int y, int z,Voxel voxel, float uvsize);
	inline void buildLeftFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
		if (isFree(x - 1, y, z, drawGroup)) buildLeftFace(mesh, x, y, z, voxel, uvsize);
	}
	void buildFrontFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel, float uvsize);
	inline void buildFrontFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
		if (isFree(x, y, z + 1, drawGroup))buildFrontFace(mesh, x, y, z, voxel, uvsize);
	}
	void buildBackFace(		ShellGeometry& mesh, int x, int y, int z, Voxel voxel, float uvsize);
	inline void buildBackFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
		if (isFree(x, y, z - 1, drawGroup))buildBackFace(mesh, x, y, z, voxel, uvsize);
	}

	friend class ChunkMeshQueue;
	friend class ChunkSectorRender;
	void buildBox(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildSortBox(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildLiquidBox(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildСrossroad(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildLiquid(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);

	void buildMesh();
	void buildSortMesh(glm::ivec3 posView);

	float heights[4] = {1.f,1.f,1.f,1.f};
};
///Chunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class Chunk:public MeshChunk {
public:
	Chunk():
		MeshChunk(){}
	

private:
	void lightFace(int x, int yT, int z, Vec4Array* face);
};
#endif

