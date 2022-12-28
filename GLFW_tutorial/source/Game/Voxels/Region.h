#ifndef REGION_H
#define REGION_H

#include "Chunk.h"
#include "Game/Loader/SaveManager.h"
#include "Game/Voxels/MasterGeneration.h"

typedef Array2d<REGION_SIZE, Chunk> Chunks;
typedef vector2d<Chunk*> ChunkPtrs;
///RegionFile---------------------------------------------
/// <summary>
///  Класс файлового взимодействия с игрой
/// </summary>
class RegionFile{
public:
	inline void init(size_t x, size_t z) { init(x, z); }
	inline void init(const glm::uvec2& begin) {
		beg_ch = begin; beg_reg = beg_ch / REGION_VEC;
		clear();
	}
	bool save();
	inline const glm::uvec2& getPosInChunk() { return beg_ch; }
	//Если чанк не сгенирирован генерирует
	inline Chunk& getChunk(const glm::uvec2& local_ch) {
		Chunk& ch = chunks_(local_ch);
		if (ch.flag.isGenerated() == 0) generate(ch);
		return ch;
	}
	inline Chunk& getChunk(size_t x, size_t z) noexcept {return getChunk(glm::uvec2(x, z));}
	inline Chunk& getChunkGlobal(const glm::uvec2& coord_ch) { return getChunk(coord_ch - beg_ch); }
	inline Chunk& getChunkGlobal(size_t x, size_t y) { return getChunkGlobal(glm::uvec2(x, y)); }
	inline Chunks& chunks() { return chunks_; };

protected:

	inline void open() {
		if (reader_.isOpen()) reader_.close();
		if (reader_.open(getPath())) for (size_t i = 0; i < chunks_.size(); i++) { reader_.readChunk(chunks_[i], i); }
		else {
			createRegionFile();
			reader_.open(getPath());
		}
	}
	void clear();

	inline std::string getPath()const { return sFile::pathRegion(beg_reg); }
	inline void createRegionFile() { createRegionFile(getPath()); }
	inline void createRegionFile(size_t x_region, size_t z_region) { createRegionFile(sFile::pathRegion(x_region, z_region)); }
	inline void createRegionFile(const std::string& path) {
		LOG("createRegionFile::create path:%s\n", path.c_str());
		WriterRLE writer_(path);
		writer_.fillNull();
	}

	glm::uvec2 beg_ch = glm::uvec2(0);
	glm::uvec2 beg_reg = glm::uvec2(0);
	Chunks chunks_;
	ReaderRLE reader_;

private:

	void generate(Chunk& ch);
};
///Region---------------------------------------------
/// <summary>
///  Оболочка для RegionFile
/// </summary>
class Region:public RegionFile {

public:

	Region() {}
	Region(const glm::uvec2& begin) {init(begin);}

	inline Chunk* getByVoxel(size_t x, size_t y, size_t z) {	return get(toLocal(x, y, z)); }
	inline Chunk* getByVoxel(const glm::uvec3& coord) {			return getByVoxel(coord.x, coord.y, coord.z); }
	/// <summary>
	/// 
	/// </summary>
	/// <param name="coord"> - Global voxel coord</param>
	/// <returns></returns>
	inline const Voxel* getVoxel(const glm::uvec3& coord) {
		Chunk* chunk = getByVoxel(coord);
		return chunk ? chunk->getGlobal(coord) : 0;
	}

private:

	//Проверяет допустимость локальных кординат
	inline bool isInCh(const glm::uvec3& local) noexcept{		return (local.x < REGION_SIZE && local.z < REGION_SIZE&& local.y == 0); }
	inline Chunk* get(const glm::uvec3& local) noexcept {		return isInCh(local) ? &getChunk(local.x, local.z) : 0; }
	inline glm::uvec3 toLocal(size_t x, size_t y, size_t z) {	return glm::uvec3(x / CHUNK_W - beg_ch.x, y / CHUNK_H, z / CHUNK_D - beg_ch.y); }
	
};

#endif