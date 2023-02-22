#ifndef GENERAL_GAME_ENTITY_H
#define GENERAL_GAME_ENTITY_H

#include "Math/Entities.h"
#include "System/LuaContext.h"

namespace vox {

	enum IdVoxel :twobyte
	{
		air = 0,
		turf,
		earth,
		sand,
		snow,
		stone,
		glass,
		glowstone,
		bedrock,
		lamp,
		oak,
		big_oak ,
		spruce ,
		jungle ,
		birch ,
		acacia ,
		grass ,
		beacon ,
		redstone,
		water ,
		leaves_oak,
		size,
	};

	
class Biom {
public:
	enum Type:size_t
	{
		plain,
		tundra,
		//mountains,
		desert,
		size,
	};

	Range<byte> r_tmpr;
	Range<byte> r_humidity;
	twobyte ground, underground;
private:
};

}
#endif
