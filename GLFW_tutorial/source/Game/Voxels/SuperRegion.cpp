#include "SuperRegion.h"

inline bool isOut(const glm::uvec2& p_ch, const glm::uvec2& c_ch,size_t size_ch) noexcept {
	return (p_ch.x > c_ch.x || p_ch.y > c_ch.y || p_ch.x+SUPREG_SIZE_CH < c_ch.x+ size_ch || p_ch.y + SUPREG_SIZE_CH < c_ch.y + size_ch);
}

void SupReg::fillSector(std::vector<Chunk*>& target, size_t size_sector, const glm::uvec2& sec_beg_ch) {
	
	if (isOut(beg_ch, sec_beg_ch, size_sector)) {
		glm::uvec2 center_rg((sec_beg_ch + glm::uvec2(size_sector / 2)) / REGION_VEC);
		translate(glm::ivec2(center_rg) - glm::ivec2(1));
	}

	//init new region
	for (size_t z = sec_beg_ch.y; z < sec_beg_ch.y + size_sector; z++) {
		for (size_t x = sec_beg_ch.x; x < sec_beg_ch.x + size_sector; x++) {
			glm::uvec2 loc_rg = (glm::uvec2(x, z) - beg_ch) / REGION_VEC;
			size_t index = toInt(loc_rg,SUPREG_SIZE);
	
			if (region[index] == 0) {
				region[index] = std::make_shared<Region>();
				region[index]->init(loc_rg * REGION_VEC+ beg_ch);
			}
			target[toInt(glm::uvec2(x, z) - sec_beg_ch, size_sector)] = &region[index]->getChunkGlobal(glm::uvec2(x, z));

		}
	}
}

void SupReg::translate(const glm::ivec2& new_beg_reg) {
	glm::ivec2 offset_rg((glm::ivec2)beg_reg- new_beg_reg);
	int offsetInd = offset_rg.y * SUPREG_SIZE + offset_rg.x;
	LOG("Translate(offsetInd = %d) from x:%d,y:%d to x:%d,y:%d\n",
		offsetInd, beg_reg.x, beg_reg.y, new_beg_reg.x, new_beg_reg.y);

	beg_reg = new_beg_reg;
	beg_ch = beg_reg * REGION_VEC;

	ptrReg region_buffer[SUPREG_VOLUME];
	for (size_t i = 0; i < SUPREG_VOLUME; i++) region_buffer[i] = 0;

	for (size_t i = 0; i < SUPREG_VOLUME; i++) {
		if (region[i] == 0)continue;
		size_t index = i + offsetInd;
		if (index < SUPREG_VOLUME) region_buffer[index] = region[i];
		else region[i]->save();
	}

	for (size_t i = 0; i < SUPREG_VOLUME; i++) region[i] = region_buffer[i];
}

//void SuperRegion::fillSector(std::vector<Chunk*>& target, size_t size, const glm::uvec2& begin) {
//	//init new region
//	if (isOut(begin_, REGION_SIZE, begin, size)) {
//		_save();
//		begin_ = begin + (size / 2 - REGION_RADIUS);
//		reset();
//		LOG("New beginRegion: x:%d,y:%d\n", begin_.x, begin_.y);
//	}
//
//	for (size_t z = begin.y; z < begin.y + size; z++) {
//		for (size_t x = begin.x; x < begin.x + size; x++) {
//			size_t index = toInt(x - begin_.x, z - begin_.y, REGION_SIZE);
//
//			if (buffer[index].isGenerated == 0) {
//
//				if (load(buffer[index], x, z) == 0) {
//					createRegionFile(x / REGION_SIZE, z / REGION_SIZE);
//				}
//
//				if (buffer[index].isGenerated == 0) {
//					generate(buffer[index], glm::uvec3(x, 0, z));
//				}
//
//			}
//
//			target[toInt(glm::uvec2(x, z) - begin, size)] = &buffer[index];
//
//		}
//	}
//}