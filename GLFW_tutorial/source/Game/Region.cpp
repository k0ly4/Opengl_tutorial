#include "Region.h"

void Region::upTarget(std::vector<Chunk*>& target, size_t size, const glm::uvec2& begin) {
	target.resize(size * size);
	//create new region
	if ((begin.x >= begin_.x && begin.y >= begin_.y && begin.x + size <= begin_.x + REGION_SIZE && begin.y + size <= begin_.y + REGION_SIZE) == 0) {
		save();
		begin_ = begin + (size / 2 - REGION_RADIUS);
		reset();
		LOG("New beginRegion: x:%d,y:%d\n", begin_.x, begin_.y);
	}

	for (size_t z = begin.y; z < begin.y + size; z++) {
		for (size_t x = begin.x; x < begin.x + size; x++) {
			size_t index = toInt(x- begin_.x, z- begin_.y, REGION_SIZE);

			if (buffer[index].isGenerated == 0) {
				if (load(buffer[index], x, z) == 0) {
					createRegion(x, z);
				}
			
				if (buffer[index].isGenerated == 0) {
					buffer[index].generate(glm::ivec3(x, 0, z));
				}

			}

			target[toInt(glm::uvec2(x, z) - begin, size)] = &buffer[index];

		}
	}
}

void Region::generateRegion(std::vector<Chunk>& chunks, const glm::uvec2& center) {
	chunks.resize(REGION_VOLUME);
	for (size_t z = 0, index = 0; z < REGION_SIZE; z++) {
		for (size_t x = 0; x < REGION_SIZE; x++, index++) {
			chunks[index].generate(glm::uvec3(center.x + x, 0, center.y + z));
		}
	}
}

bool Region::save(const std::string& path, const std::vector<Chunk>& buffer) {
	WriterRLE writer;

	if (writer.open(path) == 0) return 0;

	size_t pos_buffer[REGION_VOLUME];

	//Start pos
	const size_t start_pos = sizeof(pos_buffer);
	writer.set(start_pos);
	
	for (size_t i = 0; i < REGION_VOLUME; i++) {
		if (buffer[i].isGenerated == 0) {
			pos_buffer[i] = i == 0 ? start_pos : pos_buffer[i-1];
			continue;
		}
		writer.writeVoxels(buffer[i].getVoxels());
		pos_buffer[i] = writer.getPos();
	}

	//Pos write
	writer.write(0, pos_buffer);
	//for (size_t i = 0; i < REGION_VOLUME; i++)
		//LOG("%d\n",pos_buffer[i]);

	writer.close();
	return 1;
}

bool Region::load(Chunk& chunk, size_t x, size_t z) {
	ReaderRLE reader;

	std::string path (sSaveF::pathChunk(x / REGION_SIZE, z / REGION_SIZE));
	if (reader.open(path) == 0) return 0;

	reader.readChunk(chunk, toInt(x % REGION_SIZE, z % REGION_SIZE, REGION_SIZE));

	reader.close();
	return 1;
}

void Region::reset() {

	for (size_t z = 0; z < REGION_SIZE; z++) {
		for (size_t x = 0; x < REGION_SIZE; x++) {
			size_t index = toInt(x, z, REGION_SIZE);
			buffer[index].isGenerated = 0;
			buffer[index].setPosition(glm::uvec3(x + begin_.x, 0, z + begin_.y));
		}
	}

	for (size_t i = 0; i < buffer.size(); i++) buffer[i].setCloses(buffer);
}


