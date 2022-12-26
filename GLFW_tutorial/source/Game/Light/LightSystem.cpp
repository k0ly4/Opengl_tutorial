#include "LightSystem.h"

void LightHandle::init() {

	size_t x0 = chunks->getBegin().x * CHUNK_W, xm = (chunks->size()+ chunks->getBegin().x )* CHUNK_W ;
	size_t z0 = chunks->getBegin().y * CHUNK_D, zm = (chunks->size() + chunks->getBegin().y) * CHUNK_D ;
	//Light block
	for (size_t y = 0; y < CHUNK_H; y++) {
		for (size_t  z = z0; z < zm; z++) {
			for (size_t x = x0; x < xm; x++) {
				glm::uvec3 pos(x, y, z);
				const Voxel* voxel = chunks->getVoxel(pos);
				if (VoxelPack::isEmission(*voxel)) {
					addRGB(pos, VoxelPack::get(*voxel).emission);
				}
			}
		}
	}

	//Sun
	for (size_t z = z0; z < zm; z++) {
		for (size_t x = x0; x < xm; x++) {
			for (int y = CHUNK_H - 1; y >= 0; y--) {
				glm::uvec3 pos(x, y, z);
				const Voxel* voxel = chunks->getVoxel(pos);
				if (VoxelPack::isOpaque(*voxel)) {
					break;
				}
				chunks->getByVoxel(pos.x,pos.y,pos.z)->lightMap.setS(x % CHUNK_W, y % CHUNK_H, z % CHUNK_D, 0xF);
			}
		}
	}
	for (size_t z = z0; z < zm; z++) {
		for (size_t x = x0; x < xm; x++) {
			for (int y = CHUNK_H - 1; y >= 0; y--) {
				glm::uvec3 pos(x, y, z);
				const Voxel* voxel = chunks->getVoxel(pos);
				if (VoxelPack::isOpaque(*voxel)) {
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
				chunks->getByVoxel(pos.x,pos.y,pos.z)->lightMap.setS(x % CHUNK_W, y % CHUNK_H, z % CHUNK_D, 0xF);
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
				if (VoxelPack::isOpaque(*chunks->getVoxel(pos.x, i, pos.z))) break;
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

void LightHandle::add(const glm::ivec3& pos, Voxel voxel) {

		removeRGBS(pos);

		for (int i = pos.y - 1; i >= 0; i--) {
			solverS.remove(pos.x, i, pos.z);
			if (i == 0 || VoxelPack::isOpaque(*chunks->getVoxel(pos.x, i - 1, pos.z)))
			{
				break;
			}
		}
		solveRGBS();

		if (VoxelPack::isEmission(voxel)) {
			addRGB(pos, VoxelPack::get(voxel).emission);
			solveRGB();
		}
}

void LightHandle::chunkInit(Chunk& chunk) {
	const glm::uvec3& beg = chunk.globalPos();
	Voxels& voxels = chunk.voxels();
	//Light block
	for (size_t i = 0; i < voxels.size(); i++) {
		if (VoxelPack::isEmission(voxels[i])) {
			addRGB(beg + toCoord3(i), VoxelPack::get(voxels[i]).emission);
		}
	}
	//Sun
	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {
			for (int y = CHUNK_H - 1; y >= 0; y--) {
				if (VoxelPack::isOpaque(voxels(x, y, z))) break;
				chunk.lightMap.setS(x, y, z, 0xF);
			}
		}
	}
	for (size_t z = 0; z < CHUNK_D; z++) {
		for (size_t x = 0; x < CHUNK_W; x++) {
			for (int y = CHUNK_H - 1; y >= 0; y--) {
			
				if (VoxelPack::isOpaque(voxels(x, y, z))) break;
			
				if (
					chunk.getLight(x - 1, y, z, 3) == 0 ||
					chunk.getLight(x + 1, y, z, 3) == 0 ||
					chunk.getLight(x, y - 1, z, 3) == 0 ||
					chunk.getLight(x, y + 1, z, 3) == 0 ||
					chunk.getLight(x, y, z - 1, 3) == 0 ||
					chunk.getLight(x, y, z + 1, 3) == 0
					)
				{
					solverS.add(beg + glm::uvec3(x, y, z));
				}
				chunk.lightMap.setS(x, y, z, 0xF);
			}
		}
	}
	chunk.isInitLight = 1;
	chunk.setModified();
}