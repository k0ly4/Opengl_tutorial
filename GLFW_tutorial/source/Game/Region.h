#ifndef REGION_H
#define REGION_H

#include "Chunk.h"
///Region---------------------------------------------
/// <summary>
///  
/// </summary>
class Region {

public:

	Region() {

		buffer.resize(region_size * region_size);
		fill();
	}

	void fill(std::vector<Chunk*>& target, size_t size, const glm::uvec2& center);
	inline bool save()const { return save(getPath(), buffer); }
	static bool save(const std::string& path, const std::vector<Chunk>& chunks);

private:

	inline void fill() {
		for (size_t z = 0; z < region_size; z++) {
			for (size_t x = 0; x < region_size; x++) {
				size_t index = toInt(x, z, region_size);
				buffer[index].isGenerated = 0;
				buffer[index].setPosition(glm::uvec3(x + begin_.x, 0, z + begin_.y));
			}
		}

		for (size_t i = 0; i < buffer.size(); i++) buffer[i].setCloses(buffer);
	}

	static inline std::string getPath(const glm::uvec2& beg) {
		return DIRECTORY + std::to_string(beg.x) + "@" + std::to_string(beg.y) + EXTENSION;
	}

	inline std::string getPath()const {
		return getPath(begin_);
	}

	static void generate(std::vector<Chunk>& empty, const glm::uvec2& center);
	static bool load(std::vector<Chunk>& empty, const glm::uvec2& center);

	void generate(Chunk& chunk);
	bool load(Chunk& chunk, size_t x, size_t z);

	static const std::string DIRECTORY;
	static const std::string EXTENSION;

	static const size_t region_size = 12;
	static const size_t region_radius = region_size / 2;

	glm::uvec2 begin_ = glm::uvec2(0);
	std::vector<Chunk> buffer;
};

#endif