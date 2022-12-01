#ifndef REGION_H
#define REGION_H

#include "Chunk.h"
#include "Game/Loader/SaveManager.h"
#include "Game/Voxels/MasterGeneration.h"

///Region---------------------------------------------
/// <summary>
///  Класс файлового взимодействия с игрой
/// </summary>
class Region {

public:

	Region() {}
	Region(const glm::uvec2& begin) {
		init(begin);
	}
	inline void init(size_t x, size_t z) { init(x, z); }
	inline void init(const glm::uvec2& begin) {
		beg_ch = begin; beg_reg = beg_ch / REGION_VEC;
		clear();
	}
	bool save();

	inline const glm::uvec2& getPosInChunk() { return beg_ch;  }

	Chunk& getChunkLocal(const glm::uvec2& coord_ch);
	inline Chunk& getChunkGlobal(const glm::uvec2& coord_ch) { return getChunkLocal(coord_ch - beg_ch); }

	inline Chunk* getChunks() { return buffer; };

private:
	inline void open() {
		if (reader_.isOpen()) reader_.close();
		if (reader_.open(getPath())) { 
			for (size_t i = 0; i < REGION_VOLUME; i++) { load(buffer[i], i); }
		}
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

	bool load(Chunk& chunk, size_t x, size_t z);
	inline void load(Chunk& chunk, size_t index) {reader_.readChunk(chunk, index);}


	glm::uvec2 beg_ch = glm::uvec2(0);
	glm::uvec2 beg_reg = glm::uvec2(0);
	Chunk buffer[REGION_VOLUME];
	ReaderRLE reader_;
};

#endif