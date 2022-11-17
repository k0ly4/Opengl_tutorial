#ifndef REGION_H
#define REGION_H

#include "Chunk.h"
#include "Game/Loader/SaveManager.h"
#include "Game/Voxels/MasterGeneration.h"

struct uvecXZ {
	size_t x, z;
	operator glm::uvec2() {
		return glm::uvec2(x, z);
	}
};

///Region---------------------------------------------
/// <summary>
///  
/// </summary>
class Region {

public:

	inline void init(size_t x, size_t z) { init(x, z); }
	inline void init(const glm::uvec2& begin) {
		beg_ch = begin;
		beg_reg = beg_ch / REGION_VEC;
		setNull();
	}
	
	inline bool save()const { return save(getPath(), buffer); }

	inline bool _save()const { return _save(buffer, beg_ch);}

	inline const glm::uvec2& getPosInChunk() { return beg_ch;  }

	Chunk& getChunkLocal(const glm::uvec2& coord_ch);
	inline Chunk& getChunkGlobal(const glm::uvec2& coord_ch) { return getChunkLocal(coord_ch - beg_ch); }

	inline Chunk* getChunks() { return buffer; };

private:

	void setNull();

	inline std::string getPath()const { return sSaveF::pathRegion(beg_reg);}

	inline void createRegionFile() { createRegionFile(getPath()); }

	static inline void createRegionFile(size_t x_region, size_t z_region) { 
		createRegionFile(sSaveF::pathRegion(x_region, z_region)); 
	}
	static inline void createRegionFile(const std::string& path) {
		LOG("createRegionFile::create path:%s\n", path.c_str());
		WriterRLE writer;
		writer.open(path);
		writer.fillNull();
		writer.close();
	}

	static bool _save(const Chunk* chunks, const glm::uvec2& begin_);
	static bool save(const std::string& path, const Chunk* chunks);

	static void saveInRegion(const Chunk* buffer, size_t x_reg, size_t y_reg);

	bool load(Chunk& chunk, size_t x, size_t z);
	bool load(Chunk& chunk, size_t index);

	glm::uvec2 beg_ch = glm::uvec2(0);
	glm::uvec2 beg_reg = glm::uvec2(0);
	Chunk buffer[REGION_VOLUME];
};

#endif