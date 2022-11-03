#include "LightSystem.h"

void LightHandle::init() {

	for (int y = 0; y < chunks->size().y * CHUNK_H; y++) {
		for (int z = 0; z < chunks->size().z * CHUNK_D; z++) {
			for (int x = 0; x < chunks->size().x * CHUNK_W; x++) {
				glm::ivec3 pos(x, y, z);
				const Voxel* voxel = chunks->getVoxel(pos);
				if (voxel->id == idLight) {
					addRGB(pos, 0xF);
				}
			}
		}
	}

	for (int z = 0; z < chunks->size().z * CHUNK_D; z++) {
		for (int x = 0; x < chunks->size().x * CHUNK_W; x++) {
			for (int y = chunks->size().y * CHUNK_H - 1; y >= 0; y--) {
				glm::ivec3 pos(x, y, z);
				const Voxel* voxel = chunks->getVoxel(pos);
				if (isRender(*voxel)) {
					break;
				}
				chunks->getByVoxel(pos)->lightMap.setS(x % CHUNK_W, y % CHUNK_H, z % CHUNK_D, 0xF);
			}
		}
	}

	for (int z = 0; z < chunks->size().z * CHUNK_D; z++) {
		for (int x = 0; x < chunks->size().x * CHUNK_W; x++) {
			for (int y = chunks->size().y * CHUNK_H - 1; y >= 0; y--) {
				glm::ivec3 pos(x, y, z);
				const Voxel* voxel = chunks->getVoxel(pos);
				if (isRender(*voxel)) {
					break;
				}
				if (
					chunks->getChannelLight(x - 1, y, z, 3) == 0 ||
					chunks->getChannelLight(x + 1, y, z, 3) == 0 ||
					chunks->getChannelLight(x, y - 1, z, 3) == 0 ||
					chunks->getChannelLight(x, y + 1, z, 3) == 0 ||
					chunks->getChannelLight(x, y, z - 1, 3) == 0 ||
					chunks->getChannelLight(x, y, z + 1, 3) == 0
					) {
					solverS.add(pos);
				}
				chunks->getByVoxel(pos)->lightMap.setS(x % CHUNK_W, y % CHUNK_H, z % CHUNK_D, 0xF);
			}
		}
	}

	solveRGBS();
}

void LightHandle::remove(const glm::ivec3& pos) {
	
		removeRGB(pos);
		solveRGB();

		if (chunks->getChannelLight(pos.x, pos.y + 1, pos.z, 3) == 0xF) {
			for (int i = pos.y; i >= 0; i--) {
				if (isRender(*chunks->getVoxel(pos.x, i, pos.z))) break;
				solverS.add(pos.x, i, pos.z, 0xF);
			}
		}
		int x = pos.x, y = pos.y, z = pos.z;
		addRGBS(x, y + 1, z);
		addRGBS(x, y - 1, z);
		addRGBS(x + 1, y, z);
		addRGBS(x - 1, y, z);
		addRGBS(x, y, z + 1);
		addRGBS(x, y, z - 1);

		solveRGBS();
}

void LightHandle::add(const glm::ivec3& pos,int blockId) {

		removeRGBS(pos);

		for (int i = pos.y - 1; i >= 0; i--) {
			solverS.remove(pos.x, i, pos.z);
			if (i == 0 || isRender(*chunks->getVoxel(pos.x, i - 1, pos.z))) 
			{
				break;
			}
		}
		solveRGBS();

		if (blockId == idLight) {
			addRGB(pos, { 10,10,0 });
			solveRGB();
		}
}