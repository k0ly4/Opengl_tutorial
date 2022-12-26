#include "Region.h"
inline void  generate(Chunk& ch) {
	LOG("Generate\n");
	MasterGeneration::generateChunk(ch);
	ch.closes.setModifiedFast();
}

Chunk& Region::getChunk(const glm::uvec2& coord_ch) {
	Chunk& ch = buffer(coord_ch);
	if (ch.isGenerated == 0) generate(ch);
	return ch;
}

bool Region::save() {
	WriterRLE writer(getPath());
	if (writer.isOpen() == 0) {
		LOG("Region::save:: writer is not open path%s\n", getPath().c_str());
		return 0;
	}

	size_t pos_buffer[REGION_VOL];

	//Start pos
	writer.set(RLE::beginChunkBlock());
	
	for (size_t i = 0; i < REGION_VOL; i++) {
		if (buffer[i].isGenerated == 0) {
			pos_buffer[i] = i == 0 ? RLE::beginChunkBlock() : pos_buffer[i-1];
			continue;
		}
		writer.writeVoxels(buffer[i].voxels());
		pos_buffer[i] = writer.getPos();
	}
	//Pos write
	writer.write(0, pos_buffer);
	return 1;
}

inline void set(const glm::ivec2& coord, gChunk*& target, Chunks& source) {
	if (source.is(coord)) target = &source(coord);
}

void Region::clear() {

	for (size_t z = 0; z < REGION_SIZE; z++) 
		for (size_t x = 0; x < REGION_SIZE; x++) 
			buffer(x, z).init(x + beg_ch.x, z + beg_ch.y);
	
	open();

	//Set Closes
	for (size_t i = 0; i < buffer.size(); i++) {
		Chunk::Closes& cl = buffer[i].closes;
		cl.clear();
		glm::ivec2 coord(toCoord2(i, REGION_SIZE));

		set(glm::ivec2(coord.x-1, coord.y),			 cl.chunks[Side2D::left],	buffer);
		set(glm::ivec2(coord.x+1, coord.y),			 cl.chunks[Side2D::right],	buffer);
		set(glm::ivec2(coord.x, coord.y-1),			 cl.chunks[Side2D::bottom], buffer);
		set(glm::ivec2(coord.x, coord.y+1),			 cl.chunks[Side2D::top],	buffer);

		set(glm::ivec2(coord.x - 1, coord.y - 1),	 cl.chunks[Side2D::left_bottom],	buffer);
		set(glm::ivec2(coord.x + 1, coord.y - 1),	 cl.chunks[Side2D::right_bottom],	buffer);
		set(glm::ivec2(coord.x - 1,	coord.y + 1),	 cl.chunks[Side2D::left_top],		buffer);
		set(glm::ivec2(coord.x + 1, coord.y + 1),	 cl.chunks[Side2D::right_top],		buffer);
	}
}


