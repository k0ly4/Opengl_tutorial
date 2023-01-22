#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H
#include "Game/Voxels/Chunk.h"

///iChunkMeshCreater-------------------------------------------------------------
class iChunkMeshBuilder {
public:
	ChunkGraphic* _this = 0;
	virtual void buildOpaqueMesh() = 0;
	virtual void buildSortMesh() = 0;
private:
};

///ChunkMeshCreatorDefault-------------------------------------------------------------
//Áóäåò ðàáîòàòü òîëüêî â îäíîì ïîòîêå
class ChunkMeshBuilderDefault:public iChunkMeshBuilder {
public:
		ChunkMeshBuilderDefault() {}
		~ChunkMeshBuilderDefault() {}
		
		void buildOpaqueMesh() override;
		void buildSortMesh() override;
		
private:
		inline byte LIGHT(int x, int y, int z, int channel)noexcept { return _this->LIGHT(x, y, z, channel);}
		inline ColorU LIGHT(int x, int y, int z)noexcept			{ return _this->LIGHT(x, y, z); }
		inline byte LIGHT(const glm::ivec3& coord, int channel)		{ return LIGHT(coord.x, coord.y, coord.z, channel); }
		inline Voxel VOXEL(int x, int y, int z)noexcept				{ return _this->VOXEL(x, y, z); }
		inline glm::vec4 getChannels(int x, int y, int z) {
			return { (float)LIGHT(x,y,z,0), (float)LIGHT(x,y,z,1), (float)LIGHT(x,y,z,2), (float)LIGHT(x,y,z,3) };
		}

		void lightFaceXZ(int x, int y, int z, Vec4Array* face);
		void lightFaceYZ(int x, int y, int z, Vec4Array* face);
		void lightFaceXY(int x, int y, int z, Vec4Array* face);

		inline bool isFree(int x, int y, int z, byte drawGroup)noexcept { return _this->isFree(x, y, z, drawGroup);}
		
		void buildTopFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel);
		inline void buildTopFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel) {
			if (isFree(x, y + 1, z, drawGroup))buildTopFace(mesh, x, y, z, voxel);
		}
		void buildBottomFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel);
		inline void buildBottomFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel) {
			if (isFree(x, y - 1, z, drawGroup)) buildBottomFace(mesh, x, y, z, voxel);
		}
		void buildRightFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel);
		inline void buildRightFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel) {
			if (isFree(x + 1, y, z, drawGroup))buildRightFace(mesh, x, y, z, voxel);
		}
		void buildLeftFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel);
		inline void buildLeftFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel) {
			if (isFree(x - 1, y, z, drawGroup)) buildLeftFace(mesh, x, y, z, voxel);
		}
		void buildFrontFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel);
		inline void buildFrontFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel) {
			if (isFree(x, y, z + 1, drawGroup))buildFrontFace(mesh, x, y, z, voxel);
		}
		void buildBackFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel);
		inline void buildBackFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel) {
			if (isFree(x, y, z - 1, drawGroup))buildBackFace(mesh, x, y, z, voxel);
		}
		void buildBox(ShellGeometry& mesh,			Voxel voxel, size_t x, size_t y, size_t z);
		void buildSortBox(ShellGeometry& mesh,		Voxel voxel, size_t x, size_t y, size_t z);
		void buildLiquidBox(ShellGeometry& mesh,	Voxel voxel, size_t x, size_t y, size_t z);
		void buildÑrossroad(ShellGeometry& mesh,	Voxel voxel, size_t x, size_t y, size_t z);
		void buildLiquid(ShellGeometry& mesh,		Voxel voxel, size_t x, size_t y, size_t z);
		float heights[4] = { 1.f,1.f,1.f,1.f };
};
///sChunkMesh-------------------------------------------------------------
class sChunkMesh
{
public:

	inline static void buildOpaqueMesh(ChunkGraphic& chunk) {				
		if (creator->_this) LOG(LogError, "sChunkMesh::thread conflict\n");
			creator->_this = &chunk;
			creator->buildOpaqueMesh();
			creator->_this = 0;
	}
	inline static void buildSortMesh(ChunkGraphic& chunk) {
		if (creator->_this) LOG(LogError, "sChunkMesh::thread conflict\n");
			creator->_this = &chunk;
			creator->buildSortMesh();
			creator->_this = 0;
	}
	static void set(std::shared_ptr<iChunkMeshBuilder>  creator_) { creator = creator_; }

private:

	sChunkMesh() {}
	~sChunkMesh() {}
	static std::shared_ptr<iChunkMeshBuilder> creator;

};
#endif

