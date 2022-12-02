#ifndef SUPER_REGION_H
#define SUPER_REGION_H

#include "Region.h"
#define SUPREG_SIZE 3
#define SUPREG_SIZE_CH SUPREG_SIZE * REGION_SIZE
#define SUPREG_VOLUME SUPREG_SIZE * SUPREG_SIZE

typedef std::shared_ptr<Region> ptrReg;

//SuperRegion------------------------------------
class SupReg
{
public:

	SupReg() :beg_ch(0) {
		for (size_t i = 0; i < SUPREG_VOLUME; i++) region[i] = 0;
	}

	void fillSector(std::vector<Chunk*>& target, size_t size, const glm::uvec2& center);

	inline void save()const { for (size_t i = 0; i < SUPREG_VOLUME; i++) if (region[i] != 0)region[i]->save(); }

private:
	void setCloses(int new_region);
	void setCloses(int reg1, int reg2);
	void translate(const glm::ivec2& new_beg_reg);
	inline void initRegion(size_t index,const glm::uvec2& begin) {
		region[index] = std::make_shared<Region>(begin);
		setCloses(index);
	}

	glm::uvec2 beg_reg;
	glm::uvec2 beg_ch;

	ptrReg region[SUPREG_VOLUME];
};
#endif

