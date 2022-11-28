#include "Region.h"

inline void generate(Chunk& chunk, const glm::uvec2& pos) {
	chunk.setPosition(glm::uvec3(pos.x,0,pos.y));
	MasterGeneration::generateChunk(chunk);

}

Chunk& Region::getChunkLocal(const glm::uvec2& coord_ch) {
	size_t index = toInt(coord_ch, REGION_SIZE);
	if (buffer[index].isGenerated == 0) {
		if (load(buffer[index], index) == 0) createRegionFile();
		if (buffer[index].isGenerated == 0) {
			generate(buffer[index], beg_ch + coord_ch);
		}
	}

	return buffer[index];
}

bool Region::save(const std::string& path, const Chunk* buffer) {
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
	writer.close();
	return 1;
}



void writeRegion(const std::string& path,RegionRLE& region) {
	WriterRLE writer;
	writer.open(path);
	writer.writeRegion(region);
	writer.close();
}

void Region::saveInRegion(const Chunk* buffer, size_t x_reg, size_t z_reg) {
	//reading
	ReaderRLE reader;
	std::string path = (sSaveF::pathRegion(x_reg, z_reg));
	if (reader.open(path) == 0) createRegionFile(path);

	RegionRLE region;
	//Fill region
	// 
	//From buffer
	for (size_t i = 0; i < REGION_VOLUME; i++) {
		if (buffer[i].isGenerated == 0)continue;
		//Получаем кординаты чанка в чанках
		size_t x = buffer[i].getLocalPos().x, z = buffer[i].getLocalPos().z;
		//Проверяем принадлежность чанка региону
		if ((x / REGION_SIZE == x_reg) && (z / REGION_SIZE == z_reg)) {
			//Получаем кординаты чанка в другом регионе
			size_t i_reg = toInt(x % REGION_SIZE, z % REGION_SIZE, REGION_SIZE);
			//Записываем в region
			RLE::compress(region.c_[i_reg], buffer[i].getVoxels());
		}
		else LOG("Chunk %d,%d not belong to region\n",x,z);
	}
	//From file
	for (size_t i = 0; i < REGION_VOLUME; i++) {
		if (region.empty(i))reader.readChunk(region.c_[i], i);
	}
	reader.close();

	//Перезаписываем файл
	writeRegion(path, region);
}

bool Region::_save(const Chunk* buffer,const glm::uvec2& begin_) {
	//!!!!//
	//writing
	
	if (begin_.x % REGION_SIZE != 0) {
		LOG("_save:begin_x==%d\n", begin_.x);
		_save(buffer, glm::uvec2((begin_.x / REGION_SIZE + 1)* REGION_SIZE, begin_.y));
		_save(buffer, glm::uvec2((begin_.x / REGION_SIZE) * REGION_SIZE, begin_.y));
	}
	else if (begin_.y % REGION_SIZE != 0) {
		LOG("_save:begin_y == %d\n", begin_.y);
		_save(buffer, glm::uvec2(begin_.x,( begin_.y / REGION_SIZE + 1) * REGION_SIZE) );
		_save(buffer, glm::uvec2(begin_.x, (begin_.y / REGION_SIZE)*REGION_SIZE) );
	}
	else saveInRegion(buffer, begin_.x / REGION_SIZE, begin_.y / REGION_SIZE);
	return 1;
}

bool Region::load(Chunk& chunk, size_t x, size_t z) {
	ReaderRLE reader(sSaveF::pathRegion(x / REGION_SIZE, z / REGION_SIZE));
	if (reader.isOpen() == 0) return 0;
	reader.readChunk(chunk, toInt(x % REGION_SIZE, z % REGION_SIZE, REGION_SIZE));
	return 1;
}

bool Region::load(Chunk& chunk,size_t index) {
	
	ReaderRLE reader(getPath());
	if (reader.isOpen() == 0) return 0;
	reader.readChunk(chunk, index);
	return 1;
}

inline bool isValid(const glm::uvec2& coord) {
	return coord.x < REGION_SIZE && coord.y < REGION_SIZE;
}

inline void set(const glm::ivec2& coord, Chunk*& target, Chunk* source) {
	if (isValid(coord)) target = &source[toInt(coord, REGION_SIZE)];
}

void Region::setNull() {

	for (size_t z = 0; z < REGION_SIZE; z++) {
		for (size_t x = 0; x < REGION_SIZE; x++) {
			size_t index = toInt(x, z, REGION_SIZE);
			buffer[index].isGenerated = 0;
			buffer[index].setPosition(glm::uvec3(x + beg_ch.x, 0, z + beg_ch.y));
		}
	}
	//Set Closes
	for (size_t i = 0; i < REGION_VOLUME; i++) {
		/*buffer[i].setCloses(buffer,REGION_VOLUME)*/
		Chunk::Closes& cl = buffer[i].closes;
		cl.clear();
		glm::ivec2 coord(toCoord2(i, REGION_SIZE));
		set(glm::ivec2(coord.x-1, coord.y), cl.chunks[left], buffer);
		set(glm::ivec2(coord.x+1, coord.y), cl.chunks[right], buffer);
		set(glm::ivec2(coord.x, coord.y-1), cl.chunks[back], buffer);
		set(glm::ivec2(coord.x, coord.y+1), cl.chunks[front], buffer);
	}
}


