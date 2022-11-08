#include "Region.h"

void Region::fill(std::vector<Chunk*>& target, size_t size, const glm::uvec2& begin) {
	target.resize(size * size);
	if ((begin.x >= begin_.x && begin.y >= begin_.y && begin.x + size <= begin_.x + region_size && begin.y + size <= begin_.y + region_size) == 0) {
		save();
		begin_ = begin + size / 2 - region_radius;
		fill();
		LOG("New beginRegion: x:%d,y:%d\n", begin_.x, begin_.y);
	}
	for (size_t z = begin.y; z < begin.y + size; z++) {
		for (size_t x = begin.x; x < begin.x + size; x++) {
			size_t index = toInt(x- begin_.x, z- begin_.y, region_size);

			if (buffer[index].isGenerated == 0) {
				load(buffer[index], x, z);
				if (buffer[index].isGenerated == 0) {
					buffer[index].generate(glm::ivec3(x, 0, z));
				}
			}
			target[toInt(glm::uvec2(x, z) - begin, size)] = &buffer[index];

		}
	}
}

void Region::generate(std::vector<Chunk>& chunks, const glm::uvec2& center) {
	chunks.resize(region_size * region_size);
	for (size_t z = 0, index = 0; z < region_size; z++) {
		for (size_t x = 0; x < region_size; x++, index++) {
			chunks[index].generate(glm::uvec3(center.x + x, 0, center.y + z));
		}
	}
}

bool Region::load(std::vector<Chunk>& empty, const glm::uvec2& center) {
	ReaderChunk reader;
	if (reader.open(getPath(center)) == 0) return 0;
	
	empty.resize(region_size * region_size);
	for (size_t i = 0; i < empty.size(); i++) {
		reader.readChunk(empty[i]);
		empty[i].setPosition(glm::uvec3(center.x + i % region_size, 0, center.y + i / region_size));
		empty[i].isGenerated = 1;
	}
	reader.close();
	
	return 1;
}

bool Region::save(const std::string& path, const std::vector<Chunk>& buffer) {
	WriterChunk writer;

	if (writer.open(path) == 0) {
		LOG("Failed to save region:%s\n", path.c_str());
		return 0;
	}
	for (size_t i = 0; i < buffer.size(); i++) {
		writer.writeChunk(buffer[i]);
	}

	writer.close();
	return 1;
}

bool Region::load(Chunk& chunk, size_t x, size_t z) {
	ReaderChunk reader;
	std::string path(getPath(glm::uvec2(x / region_size, z / region_size)));
	if (reader.open(path) == 0) {
		save(path, buffer);
		return 0;
	}
	reader.readChunk(chunk, toInt(x % region_size, z % region_size, region_size));
	reader.close();
	chunk.isInitLightMap = 0;
	return 1;
}

const std::string Region::DIRECTORY = "saves\\";
const std::string Region::EXTENSION = ".region";
