#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H
#include "Graphic/Geometry.h"
#include "Game/Voxels/VoxelAtlas.h"
#include "Game/Light/LightMap.h"
#include "Scene/Convex.h"
typedef Array3d<CHUNK_W, CHUNK_H, CHUNK_D, Voxel> Voxels;
///SortableShell---------------------------------------------
/// <summary>
/// 
/// </summary>
/*template <typename object, typename key>
class SortableShell {
public:
	SortableShell(object* obj_, key value_) :
		obj(obj_), value(value_)
	{}
	SortableShell() {}

	key value;
	object* obj;

	inline bool operator > (const SortableShell& str) const
	{
		return (value > str.value);
	}

	inline bool operator < (const SortableShell& str) const
	{
		return (value < str.value);
	}

private:
}*/;




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

		inline void clear() {
			for (size_t i = 0; i < Side2D::NuN; i++) chunks[i] = nullptr;
		}

		inline void setModified(size_t index) {
			if (chunks[index]) chunks[index]->modified = 1;
		}

		inline void setModified() {
			for (size_t i = 0; i < Side2D::NuN; i++) setModified(i);
		}
		inline void setModifiedFast() {
			for (size_t i = 0; i < Side2D::left_bottom; i++) {
				if (chunks[i]) {
					//chunks[i]->isInitLightMap = 0;
					chunks[i]->modified = 1;
				}
			}
		}

		inline void setModified(size_t x, size_t y) {

			if (x == CHUNK_W - 1) {
				setModified(Side2D::right);
				if (y == CHUNK_D - 1)setModified(Side2D::right_top);
				else if (y == 0) setModified(Side2D::right_bottom);
			}
			else if (x == 0) {
				setModified(Side2D::left);
				if (y == CHUNK_D - 1)setModified(Side2D::left_top);
				else if (y == 0) setModified(Side2D::left_bottom);
			}

			if (y == CHUNK_D - 1)setModified(Side2D::top);
			else if (y == 0) setModified(Side2D::bottom);	
			
			}

			inline gChunk* get(size_t index) {return chunks[index];}

			gChunk* chunks[Side2D::NuN];

	private:
			
	};

	gChunk(): global_(0){}

	inline void init(size_t x,size_t z) {
		local_ = glm::uvec3(x, 0, z);
		global_ = local_ * CHUNK_VEC;
		isGenerated = 0;
	}

	inline void setModified(){ modified = 1; }
	//Local
	inline bool isUnVisible(size_t x, size_t y, size_t z)const { return !(!voxs.is(x, y, z) || VoxelPack::isRender(voxs(x, y, z)));}
	
	//Global
	inline const Voxel* getGlobal(const glm::uvec3& coord) const{return voxs.get(coord - glm::uvec3(global_));}
	inline Voxel* getGlobal(const glm::uvec3& coord) {return voxs.get(coord - glm::uvec3(global_));}

	inline const glm::uvec3& globalPos()const { return global_; }
	inline const glm::uvec3& localPos()const { return local_; }

	/// <summary>
	/// Изменяет заданный в глобальных кординатах coord воксель, устанавливая себе и 6 ближайшим чанкам modified в true
	/// </summary>
	bool setVoxel(const Voxel& voxel, const glm::uvec3& coord);

	inline byte getLight(const glm::uvec3& local, int channel_)		const{	return voxs.is(local) ? lightMap.get(local, channel_) : 0;}
	inline byte getLight(size_t x,size_t y,size_t z, int channel_)	const { return getLight(glm::uvec3(x, y, z), channel_);}
	inline byte getLightGlobal(const glm::uvec3& coord, int channel_)const {	return lightMap.get(coord - global_,channel_);}

	inline void setLightGlobal(const LightUint8& light, int channel_) {
		setModified();
		lightMap.set(light.pos - global_, channel_, light.light);
	}

	Voxels& voxels()				{ return voxs;}
	const Voxels& voxels()const		{ return voxs;}

	inline bool isModified()const { return modified; }

	inline void setNull() {
		isInitLight = isGenerated = 0;
		setModified();
		for (size_t i = 0; i < voxs.size(); i++) voxs[i] = Voxel(vox::air);
	}

	bool isGenerated = 0;
	bool isInitLight = 0;

	Closes closes;
	LightMap lightMap;

protected:

	static inline bool isFree(Voxel voxel, byte drawGroup) {
		return !VoxelPack::isRender(voxel) || (VoxelPack::get(voxel).drawGroup != drawGroup);
	}

	inline bool isFree(int x, int y, int z, byte drawGroup) {
		///Local test
		if (voxs.is(x, y, z)) return isFree(voxs(x, y, z),drawGroup);
		//Global
		if ((size_t)y >= CHUNK_H) return 1;//66 66 [1]-67 66
		const gChunk* chunk = closes.get( Side2D::toSideI( x , z, CHUNK_W - 1));
		if (chunk == nullptr) return 0;
		return isFree( chunk->voxels().clip(x,y,z), drawGroup);
	}
	//render
	mutable bool modified = 1;
	glm::uvec3 local_, global_;
	Voxels voxs;
};
class Chunk;

///Chunk---------------------------------------------
/// <summary>
/// 
/// </summary>
class Chunk:public gChunk {
public:
	struct SortableVoxel {
		glm::uvec3 pos;
		size_t d;
		SortableVoxel(const glm::uvec3& pos_) :pos(pos_) {}
		SortableVoxel() {}
	};

	struct ShellGeometry:public iGeometry<VoxelVertex> {
		bool needUpBuffer = 0;
	};
	Chunk():
		gChunk(), 
		curPosView(0.f)
	{
		mesh.VBO.setMode(GBO::DYNAMIC);
		mesh.EBO.setMode(GBO::DYNAMIC);
		sort_mesh.VBO.setMode(GBO::DYNAMIC);
		sort_mesh.EBO.setMode(GBO::DYNAMIC);
	}
	//render
	//void draw(const View* view, const Shader& shader);
	void drawOpaqueMesh(const Shader& shader);
	void drawSortMesh(const Shader& shader);

	ShellGeometry mesh;
	ShellGeometry sort_mesh;

private:
	glm::ivec3 curPosView;
	std::vector<SortableVoxel> sort_vox;

	unsigned char LIGHT(int x, int y, int z, int channel) {
		
		if (voxs.is(x, y, z)) return lightMap.get(x, y, z, channel);
		if ((size_t)y >= CHUNK_H) return 0;
		gChunk* chunk = closes.get(Side2D::toSideI(x, z, CHUNK_W - 1));
		return chunk != 0 ? chunk->lightMap.get(::clip(x, CHUNK_W), ::clip(y, CHUNK_H), ::clip(z, CHUNK_D), channel) : 0;
	}
	inline unsigned char LIGHT(const glm::ivec3& coord, int channel) {LIGHT(coord.x, coord.y, coord.z, channel);}

	glm::vec4 getFastLight(int x, int y, int z);
	glm::vec4 getSoftLight(int x, int y, int z);

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
	
	void getLightTop(int x, int y, int z,		LightFace& face);
	void getLightBottom(int x, int y, int z,	LightFace& face);
	void getLightRight(int x, int y, int z,		LightFace& face);
	void getLightLeft(int x, int y, int z,		LightFace& face);
	void getLightForward(int x, int y, int z,	LightFace& face);
	void getLightBack(int x, int y, int z,		LightFace& face);
	
	friend class ChunkMeshQueue;
	void buildBox(			ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildСrossroad(	ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildLiquid(		ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z);
	void buildMesh();
	void buildSortMesh(const glm::ivec3& posView);
	//void createMesh(ShellGeometry& dest, size_t x, size_t y, size_t z);
	//inline void createMesh(ShellGeometry& dest, const glm::uvec3& pos) { createMesh(dest, pos.x, pos.y, pos.z); }
};
#endif

