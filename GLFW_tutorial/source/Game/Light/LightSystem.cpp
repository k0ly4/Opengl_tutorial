#include "LightSystem.h"

void LightHandle::remove(const glm::ivec3& pos) {
		
		removeRGBS(pos);
		//solveRGB();
		//sun
		if (region->getChannelLight(pos.x, pos.y + 1, pos.z, 3) == 0xF) {
			for (int i = pos.y; i > -1; i--) {
				if (VoxPack::isOpaque(*region->getVoxel(pos.x, i, pos.z))) break;
				solverS.add(pos.x, i, pos.z, 0xF);
			}
		}
		//rgb
		/*int x = pos.x, y = pos.y, z = pos.z;
		addRGBS(x, y + 1, z);
		addRGBS(x, y - 1, z);
		addRGBS(x + 1, y, z);
		addRGBS(x - 1, y, z);
		addRGBS(x, y, z + 1);
		addRGBS(x, y, z - 1);*/

		solveRGBS();
}

void LightHandle::add(const glm::ivec3& pos, Voxel voxel) {

		removeRGBS(pos);
		//solve s
		for (int i = pos.y - 1;i>-1; i--) {
			if (VoxPack::isOpaque(*region->getVoxel(pos.x, i, pos.z)))break;
			solverS.remove(pos.x, i, pos.z);
		}
		//solve rgb
		if (VoxPack::isEmission(voxel)) {
			addRGB(pos, VoxPack::get(voxel).emission);
			
		}
		solveRGBS();
}
inline bool isLess(Chunk& chunk,size_t x,size_t y,size_t z) {
	return chunk.voxels().is(x,y,z)&& (chunk.lightMap.getS(x, y, z) < 14) && VoxPack::isTransparent(chunk.voxels()(x, y, z));
}
void LightHandle::chunkInit(Chunk& chunk) {
	glm::uvec3 beg(chunk.posVx());
	Voxels& voxels = chunk.voxels();
	//Light block
	for (size_t i = 0; i < voxels.size(); i++) {
		if (VoxPack::isEmission(voxels[i])) {
			addRGB(beg + toCoord3(i), VoxPack::get(voxels[i]).emission);
		}
	}
	//Sun
	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {
			for (int y = CHUNK_H - 1; y >= 0; y--) {
				if (VoxPack::isOpaque(voxels(x, y, z))) break;
				chunk.lightMap.setS(x, y, z, 0xF);
			}
		}
	}
	chunk.flag.status = StateChunk::s_lighting_phase_solving;
	for (size_t i = 0; i < voxels.size(); i++) {
		if (chunk.lightMap.getS(i) == 0xF) {
			
			glm::uvec3 pos(voxels.coord(i));
			if (
				isLess(chunk,pos.x - 1,	pos.y,		pos.z)		||
				isLess(chunk,pos.x + 1,	pos.y,		pos.z)		||
				isLess(chunk,pos.x,		pos.y,		pos.z - 1)  ||
				isLess(chunk,pos.x,		pos.y,		pos.z + 1)
				)
			{
				solverS.add(beg + pos);
				solverS.solve();
			}
		}
	}
	chunk.flag.toMeshLevel();
	chunk.closes.modify();
}