#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H
#include "Graphic/Geometry.h"
#include "Game/Voxels/VoxelAtlas.h"
#include "Game/Light/LightMap.h"
#include "Scene/Convex.h"
#include <set>
typedef Array3d<CHUNK_W, CHUNK_H, CHUNK_D, Voxel> Voxels;

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
		modify();
		generated = 1;
		is_init_light = 0;
	}
	//generated = 0
	inline void offInit()noexcept { generated = 0;}
	/*modify();
	is_init_light = 1;*/
	inline void initLight()noexcept {
		is_init_light = 1;
		modify();
	}
	/// modifiedAlpha = 1; 
	inline void modifyAlpha() { modifiedAlpha=1; }
private:

	friend class gChunk;
	friend class Chunk;
	bool modifiedAlpha = 0;
	bool is_init_light = 0;
	bool generated = 0;
	bool modified = 1;

};
///ContainerNonStaticVoxel---------------------------------------------
/// <summary>
/// 
/// </summary>
class ContainerNonStaticVoxel {
public:
	void log() {
		LOG("ContainerNonStaticVoxel:");
		for (size_t i : voxs_) {
			LOG("%d, ", i);
		}
		LOG("\n");
	}
	//Заполняется только при добавлении новых блоков
	std::set<size_t> voxs_;
private:
	
};
///Chunk---------------------------------------------
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
		inline void modify(size_t index) { if (chunks[index]) chunks[index]->flag.modify(); }
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
		
	private:
		gChunk* chunks[Side2D::NuN];
	};

	struct SortableVoxel {
		glm::uvec3 pos;
		size_t d;
		SortableVoxel(const glm::uvec3& pos_) :pos(pos_) {}
		SortableVoxel() {}
	};

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
	bool setVoxel(Voxel vx, glm::uvec3 coord);

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

	ContainerNonStaticVoxel nonStatic;

protected:
	glm::ivec3 posView_;
	std::vector<SortableVoxel> sort_vox;

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
class Chunk;

///Chunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class Chunk:public gChunk {
public:
	struct Vec4M {
		union
		{
			struct { glm::vec4 v; };
			struct { float m[4]; };
		};
		Vec4M(const glm::vec4& v_) :v(v_) {}
	};

	struct LightFace {
		union
		{
			glm::vec4 l0;
			struct { float l0m[4]; };
		};
		union
		{
			glm::vec4 l1;
			struct { float l1m[4]; };
		};
		union
		{
			glm::vec4 l2;
			struct { float l2m[4]; };
		};
		union
		{
			glm::vec4 l3;
			struct { float l3m[4]; };
		};
	};

	struct ShellGeometry:public iGeometry<VoxelVertex> {
		bool needUpBuffer = 0;
	};
	Chunk():
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
	

private:

	glm::ivec3 posView_;
	std::vector<SortableVoxel> sort_vox;

	unsigned char LIGHT(int x, int y, int z, int channel) {
		
		if (voxs.is(x, y, z)) return lightMap.get(x, y, z, channel);
		if ((size_t)y >= CHUNK_H) return 0;
		gChunk* chunk = closes[Side2D::toSideI(x, z, CHUNK_W - 1)];
		return chunk != 0 ? chunk->lightMap.get(::clip(x, CHUNK_W), ::clip(y, CHUNK_H), ::clip(z, CHUNK_D), channel) : 0;
	}
	inline unsigned char LIGHT(const glm::ivec3& coord, int channel) {LIGHT(coord.x, coord.y, coord.z, channel);}

	glm::vec4 getFastLight(int x, int y, int z);

	void getLightTop(int x, int y, int z,		LightFace& face);
	void getLightBottom(int x, int y, int z,	LightFace& face);
	void getLightRight(int x, int y, int z,		LightFace& face);
	void getLightLeft(int x, int y, int z,		LightFace& face);
	void getLightForward(int x, int y, int z,	LightFace& face);
	void getLightBack(int x, int y, int z,		LightFace& face);

	void buildTopFace(		ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize);
	void buildBottomFace(	ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize);
	void buildLeftFace(		ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize);
	void buildRightFace(	ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize);
	void buildFrontFace(	ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize);
	void buildBackFace(		ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize);

	friend class ChunkMeshQueue;
	void buildBox(			ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildSortBox(		ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildСrossroad(	ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildLiquid(		ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);

	void buildMesh();
	void buildSortMesh(glm::ivec3 posView);
};
#endif

