#include "SuperRegion.h"
//Проверка выхода сектора из региона
inline bool isOut(const glm::uvec2& p_ch, const glm::uvec2& c_ch,size_t size_ch) noexcept {
	return (p_ch.x > c_ch.x || p_ch.y > c_ch.y || p_ch.x+ SUPREG_VOL_CH < c_ch.x+ size_ch || p_ch.y + SUPREG_VOL_CH < c_ch.y + size_ch);
}

void SupReg::fillSector(ChunkPtrs& sec, size_t sec_size, const glm::uvec2& sec_beg_ch) {

	if (isOut(beg_ch, sec_beg_ch, sec_size)) {
		//Установка нового центра
		glm::uvec2 center_rg((sec_beg_ch + glm::uvec2(sec_size / 2)) / REGION_VEC);
		translate(glm::ivec2(center_rg) - glm::ivec2(1));
	}
	//init new region
	for (size_t z = sec_beg_ch.y; z < sec_beg_ch.y + sec_size; z++) {
		for (size_t x = sec_beg_ch.x; x < sec_beg_ch.x + sec_size; x++) {
			glm::uvec2 pos(x, z);
			glm::uvec2 loc_rg = (pos - beg_ch) / REGION_VEC;
			size_t ind = toInt(loc_rg,SUPREG_SIZE);
			if (reg[ind] == 0) initRegion(ind, loc_rg * REGION_VEC + beg_ch);
			sec[toInt(pos - sec_beg_ch, sec_size)] = &reg[ind]->getChunkGlobal(pos);
		}
	}
}



void SupReg::translate(const glm::ivec2& new_beg_reg) {
	//Вычисление сдвига по индексу
	glm::ivec2 offset_rg((glm::ivec2)beg_reg - new_beg_reg);
	int offsetInd = offset_rg.y * SUPREG_SIZE + offset_rg.x;
	LOG("Translate(offsetInd = %d) from x:%d,y:%d to x:%d,y:%d\n",
		offsetInd, beg_reg.x, beg_reg.y, new_beg_reg.x, new_beg_reg.y);

	beg_reg = new_beg_reg;
	beg_ch = beg_reg * REGION_VEC;

	Regions reg_buff;
	reg_buff.setNull();
	//Сдвиг
	for (size_t i = 0; i < SUPREG_VOL; i++) {
		if (reg[i] == 0)continue;
		size_t index = i + offsetInd;
		if (index < SUPREG_VOL) reg_buff[index] = reg[i];
		else reg[i]->save();
	}
	reg = reg_buff;
}

void setCloses(size_t x, size_t y, size_t cur, Regions& r) {
	Chunks& reg= r[cur]->chunks();
	//left
	if (r.is(x - 1, y)&& r(x - 1, y)) {
		Chunks& reg_ = r(x - 1, y)->chunks();
		for (size_t i = 0; i < REGION_SIZE; i++) {
			Chunk& ch =		reg(0, i);
			Chunk& ch_ =	reg_(REGION_SIZE - 1, i);
			ch.	closes.chunks[Side2D::left] = &ch_;
			ch_.closes.chunks[Side2D::right] = &ch;
			ch.setModified();
			ch_.setModified();
		}
	}
	//right
	if (r.is(x +1, y) && r(x + 1, y)) {
		Chunks& reg_ = r(x + 1, y)->chunks();
		for (size_t i = 0; i < REGION_SIZE; i++) {
			Chunk& ch =		reg(REGION_SIZE - 1, i);
			Chunk& ch_ =	reg_(0, i);
			ch.closes.chunks[Side2D::right] = &ch_;
			ch_.closes.chunks[Side2D::left] = &ch;
			ch.setModified();
			ch_.setModified();
		}
	}
	//bottom
	if (r.is(x, y-1) && r(x, y - 1)) {
		Chunks& reg_ = r(x, y-1)->chunks();
		for (size_t i = 0; i < REGION_SIZE; i++) {
			Chunk& ch =		reg(i, 0);
			Chunk& ch_ =	reg_(i, REGION_SIZE - 1);
			ch.closes.chunks[Side2D::bottom] = &ch_;
			ch_.closes.chunks[Side2D::top] = &ch;
			ch.setModified();
			ch_.setModified();
		}
	}
	//top
	if (r.is(x, y + 1) && r(x, y + 1)) {
		Chunks& reg_ = r(x, y + 1)->chunks();
		for (size_t i = 0; i < REGION_SIZE; i++) {
			Chunk& ch =		reg(i, REGION_SIZE - 1);
			Chunk& ch_ =	reg_(i, 0);
			ch.closes.chunks[Side2D::top] = &ch_;
			ch_.closes.chunks[Side2D::bottom] = &ch;
			ch.setModified();
			ch_.setModified();
		}
	}
}
void SupReg::setCloses(int new_region) {
	glm::ivec2 coord(toCoord2(new_region, SUPREG_SIZE));
	::setCloses(coord.x, coord.y, new_region, reg);
}