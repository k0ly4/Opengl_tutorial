#include "SuperRegion.h"
//Проверка выхода сектора из региона
inline bool isOut(const glm::uvec2& p_ch, const glm::uvec2& c_ch,size_t size_ch) noexcept {
	return (p_ch.x > c_ch.x || p_ch.y > c_ch.y || p_ch.x+SUPREG_SIZE_CH < c_ch.x+ size_ch || p_ch.y + SUPREG_SIZE_CH < c_ch.y + size_ch);
}

void SupReg::fillSector(std::vector<Chunk*>& target, size_t size_sector, const glm::uvec2& sec_beg_ch) {

	if (isOut(beg_ch, sec_beg_ch, size_sector)) {
		//Установка нового центра
		glm::uvec2 center_rg((sec_beg_ch + glm::uvec2(size_sector / 2)) / REGION_VEC);
		translate(glm::ivec2(center_rg) - glm::ivec2(1));
	}
	//init new region
	for (size_t z = sec_beg_ch.y; z < sec_beg_ch.y + size_sector; z++) {
		for (size_t x = sec_beg_ch.x; x < sec_beg_ch.x + size_sector; x++) {
			glm::uvec2 loc_rg = (glm::uvec2(x, z) - beg_ch) / REGION_VEC;
			size_t index = toInt(loc_rg,SUPREG_SIZE);
	
			if (region[index] == 0) 
				region[index] = std::make_shared<Region>(loc_rg * REGION_VEC + beg_ch);
			target[toInt(glm::uvec2(x, z) - sec_beg_ch, size_sector)] = &region[index]->getChunkGlobal(glm::uvec2(x, z));

		}
	}
}

void SupReg::setCloses(int prev_i) {
	// 678
	// 345
	// 012
	Chunk* cur = region[4]->getChunks();
	Chunk* prev = region[prev_i]->getChunks();
	//Back
	if (prev_i == 1) {
		for (size_t i = 0; i < REGION_SIZE; i++) {
			cur[toInt(i, 0, REGION_SIZE)].closes.chunks[back] = &prev[toInt(i, REGION_SIZE - 1, REGION_SIZE)];
		}
	}
	//Left
	else if (prev_i == 3) {
		for (size_t i = 0; i < REGION_SIZE; i++) {
			cur[toInt(0, i, REGION_SIZE)].closes.chunks[left] = &prev[toInt(REGION_SIZE - 1, i, REGION_SIZE)];
		}
	}
	//Right
	else if (prev_i == 5) {
		for (size_t i = 0; i < REGION_SIZE; i++) {
			cur[toInt(REGION_SIZE - 1, i, REGION_SIZE)].closes.chunks[right] = &prev[toInt(0, i, REGION_SIZE)];
		}
	}
	//Front
	else if (prev_i == 7) {
		for (size_t i = 0; i < REGION_SIZE; i++) {
			cur[toInt(i, REGION_SIZE - 1, REGION_SIZE)].closes.chunks[front] = &prev[toInt(i, 0, REGION_SIZE)];
		}
	}
	else LOG("SupReg::setCloses::Diagonal %d\n", prev_i);
}

void SupReg::translate(const glm::ivec2& new_beg_reg) {
	//Вычисление сдвига по индексу
	glm::ivec2 offset_rg((glm::ivec2)beg_reg - new_beg_reg);
	int offsetInd = offset_rg.y * SUPREG_SIZE + offset_rg.x;
	LOG("Translate(offsetInd = %d) from x:%d,y:%d to x:%d,y:%d\n",
		offsetInd, beg_reg.x, beg_reg.y, new_beg_reg.x, new_beg_reg.y);

	beg_reg = new_beg_reg;
	beg_ch = beg_reg * REGION_VEC;

	ptrReg region_buffer[SUPREG_VOLUME];
	for (size_t i = 0; i < SUPREG_VOLUME; i++) region_buffer[i] = 0;
	//Сдвиг
	for (size_t i = 0; i < SUPREG_VOLUME; i++) {
		if (region[i] == 0)continue;
		size_t index = i + offsetInd;
		if (index < SUPREG_VOLUME) region_buffer[index] = region[i];
		else region[i]->save();
	}

	for (size_t i = 0; i < SUPREG_VOLUME; i++) region[i] = region_buffer[i];
	setCloses(4 + offsetInd);
}