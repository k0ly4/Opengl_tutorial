#ifndef SUPER_REGION_H
#define SUPER_REGION_H

#include "Region.h"


typedef std::shared_ptr<Region> RegPtr;
typedef Array2d<SUPREG_SIZE, RegPtr> Regions;

//SuperRegion--------------------------------------------------------
class SupReg
{
public:
	SupReg() :beg_ch(0) { reg.setNull(); }
	void fillSector(ChunkPtrs& sec, size_t size, const glm::uvec2& center);
	inline void save() { for (size_t i = 0; i < reg.size(); i++) if (reg[i]) reg[i]->save(); }
protected:

	void setCloses(int new_region);
	void translate(const glm::ivec2& new_beg_reg);
	inline void initRegion(size_t index,const glm::uvec2& begin) {
		reg[index] = std::make_shared<Region>(begin);
		setCloses(index);
	}

	glm::uvec2 beg_reg;
	glm::uvec2 beg_ch;

	Regions reg;
};

#endif

