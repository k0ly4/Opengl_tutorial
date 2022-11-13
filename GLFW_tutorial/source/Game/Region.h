#ifndef REGION_H
#define REGION_H

#include "Chunk.h"
#include "Game/SaveManager.h"

///Region---------------------------------------------
/// <summary>
///  
/// </summary>
class Region {

public:

	Region() {

		buffer.resize(REGION_VOLUME);
		reset();
	}

	void upTarget(std::vector<Chunk*>& target, size_t size, const glm::uvec2& center);

	inline bool save()const { return save(getPath(), buffer); }
	static bool save(const std::string& path, const std::vector<Chunk>& chunks);

private:

	void reset();

	inline std::string getPath()const { return sSaveF::pathChunk(begin_);}

	static void generateRegion(std::vector<Chunk>& empty, const glm::uvec2& center);

	inline void createRegion(size_t x_chunk, size_t z_chunk) {
		std::string path = sSaveF::pathChunk(x_chunk / REGION_SIZE, z_chunk / REGION_SIZE);
		save(path, buffer);
		LOG("LoadFromRle::create path:%s\n", path.c_str());
	}
	
	bool load(Chunk& chunk, size_t x, size_t z);

	glm::uvec2 begin_ = glm::uvec2(0);
	std::vector<Chunk> buffer; 
};

#endif