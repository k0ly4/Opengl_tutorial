#include "Region.h"

inline void generate(Chunk& chunk, const glm::uvec2& pos) {
	LOG("Generate\n");
	chunk.setPosition(glm::uvec3(pos.x,0,pos.y));
	MasterGeneration::generateChunk(chunk);
}

Chunk& Region::getChunkLocal(const glm::uvec2& coord_ch) {
	size_t index = toInt(coord_ch, REGION_SIZE);
	if (buffer[index].isGenerated == 0) generate(buffer[index], beg_ch + coord_ch);
	return buffer[index];
}

bool Region::save() {
	WriterRLE writer(getPath());
	if (writer.isOpen() == 0) {
		LOG("Region::save:: writer is not open path%s\n", getPath().c_str());
		return 0;
	}

	size_t pos_buffer[REGION_VOLUME];

	//Start pos
	writer.set(RLE::beginChunkBlock());
	
	for (size_t i = 0; i < REGION_VOLUME; i++) {
		if (buffer[i].isGenerated == 0) {
			pos_buffer[i] = i == 0 ? RLE::beginChunkBlock() : pos_buffer[i-1];
			continue;
		}
		writer.writeVoxels(buffer[i].getVoxels());
		pos_buffer[i] = writer.getPos();
	}
	//Pos write
	writer.write(0, pos_buffer);
	return 1;
}


bool Region::load(Chunk& chunk, size_t x, size_t z) {
	std::string path(sFile::pathRegion(x / REGION_SIZE, z / REGION_SIZE));
	reader_.readChunk(chunk, toInt(x % REGION_SIZE, z % REGION_SIZE, REGION_SIZE));
	return 1;
}

inline bool isValid(const glm::uvec2& coord) {
	return coord.x < REGION_SIZE && coord.y < REGION_SIZE;
}

inline void set(const glm::ivec2& coord, gChunk*& target, Chunk* source) {
	if (isValid(coord)) target = &source[toInt(coord, REGION_SIZE)];
}

void Region::clear() {

	for (size_t z = 0; z < REGION_SIZE; z++) {
		for (size_t x = 0; x < REGION_SIZE; x++) {
			size_t index = toInt(x, z, REGION_SIZE);
			buffer[index].isGenerated = 0;
			buffer[index].setPosition(glm::uvec3(x + beg_ch.x, 0, z + beg_ch.y));
		}
	}
	open();

	//Set Closes
	for (size_t i = 0; i < REGION_VOLUME; i++) {
		Chunk::Closes& cl = buffer[i].closes;
		cl.clear();
		glm::ivec2 coord(toCoord2(i, REGION_SIZE));
		set(glm::ivec2(coord.x-1, coord.y), cl.chunks[left], buffer);
		set(glm::ivec2(coord.x+1, coord.y), cl.chunks[right], buffer);
		set(glm::ivec2(coord.x, coord.y-1), cl.chunks[back], buffer);
		set(glm::ivec2(coord.x, coord.y+1), cl.chunks[front], buffer);
	}
}


