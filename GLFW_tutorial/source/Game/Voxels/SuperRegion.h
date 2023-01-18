#ifndef SUPER_REGION_H
#define SUPER_REGION_H

#include "Region.h"

typedef std::shared_ptr<Region> RegPtr;
typedef Array2d<SUPREG_SIZE, RegPtr> Regions;
//SuperRegion--------------------------------------------------------
class fSupReg
{
public:

	fSupReg() :beg_ch(0) { reg.setNull(); }
	void fillSector(ChunkPtrs& sec, size_t size, const glm::uvec2& center);
	inline void save() { for (size_t i = 0; i < reg.size(); i++) if (reg[i]) reg[i]->save(); }
protected:

	void setCloses(int new_region);
	void translate(const glm::ivec2& new_beg_reg);
	inline void initRegion(size_t index,const glm::uvec2& begin) {
		reg[index] = std::make_shared<Region>(begin);
		setCloses(index);
	}

	glm::uvec2 beg_reg;
	glm::uvec2 beg_ch;

	Regions reg;
};
//SuperRegion--------------------------------------------------------
class SupReg:public fSupReg {
public:
	SupReg () : fSupReg() {}
	//Если не загружен возвращает ноль
	inline RegPtr getByChunk(size_t x, size_t z) {
		RegPtr* region = reg.get(chToLocalReg(x, z));
		return region == 0 ? 0 : (*region);
	}
	//Если не сгенерирован возвращает ноль
	inline Chunk* getChunk(size_t x, size_t z) {
		RegPtr region = getByChunk(x,z);
		if (region == 0)return 0;
		Chunk* ch = region->getGlobal({ x,z });
		return ch->flag.isGenerated()? ch:0;
	}
	//Если не сгенерирован возвращает ноль
	inline Chunk* getByVoxel(size_t x, size_t y, size_t z) { 
		if (y >= CHUNK_H)return 0;
		return getChunk(x / CHUNK_W, z / CHUNK_D);
	}
	inline Chunk* getByVoxel(const glm::uvec3& coord) { return getByVoxel(coord.x, coord.y, coord.z); }
	/// <summary>
	/// 
	/// </summary>
	/// <param name="coord"> - Global voxel coord</param>
	/// <returns></returns>
	inline const Voxel* getVoxel(const glm::uvec3& coord) {
		Chunk* chunk = getByVoxel(coord);
		return chunk ? chunk->getGlobal(coord) : 0;
	}
	inline const Voxel* getVoxel(int x, int y, int z) { return  getVoxel(glm::uvec3(x, y, z)); }
	inline bool isObstacle(const glm::uvec3& coord) {
		const Voxel* voxel = getVoxel(coord);
		return (voxel && VoxPack::isSolid(*voxel));
	}

private:

	inline glm::uvec2 chToLocalReg(size_t x, size_t z) { return { (x - beg_ch.x)/REGION_SIZE,(z - beg_ch.y)/REGION_SIZE }; }
	//inline glm::uvec2 toLocalCh(size_t x, size_t z) { return { x - beg_ch.x,z - beg_ch.y };}

};
#endif

