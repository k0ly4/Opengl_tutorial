#include "LightSolver.h"

void LightSolver::add(const glm::ivec3& pos, unsigned char emission) {

	if (emission <= 1)
		return;

	LightUint8 entry(pos, emission);
	addqueue.push(entry);

	Chunk* chunk = chunks->getByVoxel(entry.pos.x, entry.pos.y, entry.pos.z);
	chunk->setLightGlobal(entry, channel);
}

void LightSolver::add(const glm::ivec3& pos) {
	add(pos, chunks->getChannelLight(pos, channel));
}

void LightSolver::remove(const glm::ivec3& pos) {
	Chunk* chunk = chunks->getByVoxel(pos.x, pos.y,pos.z);
	if (chunk == nullptr) return;

	unsigned char light = chunk->getLightGlobal(pos, channel);
	if (light == 0) return;

	LightUint8 entry(pos, light);
	remqueue.push(entry);
	chunk->setLightGlobal(LightUint8(entry.pos,0), channel);
}

void LightSolver::solve() {

	const glm::ivec3 coords[] = {
			{0, 0, 1},
			{0, 0,-1},
			{0, 1, 0},
			{0,-1, 0},
			{1, 0, 0},
			{-1,0, 0}
	};

	while (!remqueue.empty()) {
		LightUint8 entry(remqueue.front());
		remqueue.pop();

		for (size_t i = 0; i < 6; i++) {
			glm::uvec3 pos = glm::ivec3(entry.pos) + coords[i];
			Chunk* chunk = chunks->getByVoxel(pos.x, pos.y,pos.z);
			if (chunk == 0) continue;

			unsigned char light = chunks->getChannelLight(pos, channel);
			if (light >= entry.light) {
				addqueue.push(LightUint8(pos,light));
			}
			else if (light != 0 && light == entry.light - 1) {
				remqueue.push(LightUint8(pos, light));
				chunk->setLightGlobal(LightUint8(pos, 0), channel);
			}

		}
	}

	while (!addqueue.empty()) {
		LightUint8 entry(addqueue.front());
		addqueue.pop();
		if (entry.light <= 1)
			continue;

		for (size_t i = 0; i < 6; i++) {
			glm::ivec3 pos = glm::ivec3(entry.pos) + coords[i];
			Chunk* chunk = chunks->getByVoxel(pos);
			if (chunk == 0) continue;

			const Voxel* v = chunks->getVoxel(pos);
			if (v == 0) { 
				LOG(LogError, "Voxel* == 0\n");
				continue; 
			}
			if (VoxPack::isOpaque(*v) == 0 &&
				chunks->getChannelLight(pos, channel) + 1 < entry.light) {

					LightUint8 outer(pos, entry.light - 1);
					addqueue.push(outer);
					chunk->setLightGlobal(outer, channel);
			}
			
		}
	}
}
