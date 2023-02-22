#include "Region.h"

void  RegionFile::generate(Chunk& ch) {
	MasterGeneration::generateChunk(ch);
}

bool RegionFile::save() {
	WriterRLE writer(getPath());
	if (writer.isOpen() == 0) {
		LOG("Region::save:: writer is not open path%s\n", getPath().c_str());
		return 0;
	}

	size_t pos_buffer[REGION_VOL];

	//Start pos
	writer.set(RLE::beginChunkBlock());
	
	for (size_t i = 0; i < REGION_VOL; i++) {
		if (chunks_[i].flag.isGenerated() == 0) {
			pos_buffer[i] = i == 0 ? RLE::beginChunkBlock() : pos_buffer[i-1];
			continue;
		}
		writer.writeVoxels(chunks_[i].voxels());
		pos_buffer[i] = writer.getPos();
	}
	//Pos write
	writer.write(0, pos_buffer);
	return 1;
}

inline void set(const glm::ivec2& coord, gChunk*& target, Chunks& source) { if (source.is(coord)) target = &source(coord);}

void RegionFile::clear() {

	for (size_t z = 0; z < REGION_SIZE; z++) 
		for (size_t x = 0; x < REGION_SIZE; x++) 
			chunks_(x, z).init(x + beg_ch.x, z + beg_ch.y);
	
	open();

	//Set Closes
	for (size_t i = 0; i < chunks_.size(); i++) {
		Chunk::Closes& cl = chunks_[i].closes;
		cl.clear();
		glm::ivec2 coord(toCoord2(i, REGION_SIZE));

		set(glm::ivec2(coord.x-1, coord.y),			 cl[Side2D::left],			chunks_);
		set(glm::ivec2(coord.x+1, coord.y),			 cl[Side2D::right],			chunks_);
		set(glm::ivec2(coord.x, coord.y-1),			 cl[Side2D::bottom],		chunks_);
		set(glm::ivec2(coord.x, coord.y+1),			 cl[Side2D::top],			chunks_);

		set(glm::ivec2(coord.x - 1, coord.y - 1),	 cl[Side2D::left_bottom],	chunks_);
		set(glm::ivec2(coord.x + 1, coord.y - 1),	 cl[Side2D::right_bottom],	chunks_);
		set(glm::ivec2(coord.x - 1,	coord.y + 1),	 cl[Side2D::left_top],		chunks_);
		set(glm::ivec2(coord.x + 1, coord.y + 1),	 cl[Side2D::right_top],		chunks_);
	}
}


