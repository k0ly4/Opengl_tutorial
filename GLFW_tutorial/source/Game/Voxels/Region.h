#ifndef REGION_H
#define REGION_H

#include "Chunk.h"
#include "Game/Loader/SaveManager.h"
#include "Game/Voxels/MasterGeneration.h"

typedef Array2d<REGION_SIZE, Chunk> Chunks;
typedef vector2d<Chunk*> ChunkPtrs;
///Region---------------------------------------------
/// <summary>
///  Класс файлового взимодействия с игрой
/// </summary>
class Region {

public:

	Region() {}
	Region(const glm::uvec2& begin) {init(begin);}

	inline void init(size_t x, size_t z) { init(x, z); }
	inline void init(const glm::uvec2& begin) {
		beg_ch = begin; beg_reg = beg_ch / REGION_VEC;
		clear();
	}
	bool save();

	inline const glm::uvec2& getPosInChunk() {					return beg_ch;  }

	Chunk& getChunk(const glm::uvec2& coord_ch);
	inline Chunk& getChunkGlobal(const glm::uvec2& coord_ch) {	return getChunk(coord_ch - beg_ch); }
	inline Chunk& geChunkGlobal(size_t x, size_t y) {			return getChunkGlobal(glm::uvec2(x,y)); }

	inline Chunks& chunks() { return buffer; };

private:

	inline void open() {
		if (reader_.isOpen()) reader_.close();
		if (reader_.open(getPath())) for (size_t i = 0; i < buffer.size(); i++) { reader_.readChunk(buffer[i], i); }
		else {
			createRegionFile();
			reader_.open(getPath());
		}
	}
	void clear();

	inline std::string getPath()const { return sFile::pathRegion(beg_reg);}
	inline void createRegionFile() { createRegionFile(getPath()); }
	inline void createRegionFile(size_t x_region, size_t z_region) { createRegionFile(sFile::pathRegion(x_region, z_region)); }
	inline void createRegionFile(const std::string& path) {
		LOG("createRegionFile::create path:%s\n", path.c_str());
		WriterRLE writer_(path);
		writer_.fillNull();
	}
	glm::uvec2 beg_ch = glm::uvec2(0);
	glm::uvec2 beg_reg = glm::uvec2(0);
	Chunks buffer;
	ReaderRLE reader_;
};

#endif