#include "LightSolver.h"
//Добавляем эмиссию(emission > 1)
void LightSolver::add(const glm::ivec3& pos, byte emission) {
	if (emission < 2) return;
	Chunk* chunk = chunks->getByVoxel(pos);
	LightUint8 entry(pos, emission);
	addqueue.push(entry);
	chunk->setLightGlobal(entry, channel);
}
//Обнуляем
void LightSolver::remove(const glm::ivec3& pos) {
	Chunk* chunk = chunks->getByVoxel(pos.x, pos.y,pos.z);
	byte light = chunk->getLightGlobal(pos, channel);
	//if (light == 0) return;
	remqueue.push(LightUint8(pos, light));
	chunk->setLightGlobal(LightUint8(pos,0), channel);
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
			glm::uvec3 pos(glm::ivec3(entry.pos) + coords[i]);
			Chunk* chunk = chunks->getByVoxel(pos);
			if (chunk == 0) continue;

			byte light = chunks->getChannelLight(pos, channel);
			if (light >= entry.light) addqueue.push(LightUint8(pos,light));
			else if ((light!=0) && (light == entry.light - 1)) {
				remqueue.push(LightUint8(pos, light));
				chunk->setLightGlobal(LightUint8(pos, 0), channel);
			}

		}
	}

	while (!addqueue.empty()) {
		LightUint8 entry(addqueue.front());
		addqueue.pop();
		if (entry.light == 0) continue;

		for (size_t i = 0; i < 6; i++) {
			glm::uvec3 pos(glm::ivec3(entry.pos) + coords[i]);
			Chunk* chunk = chunks->getByVoxel(pos);
			if (chunk == 0) continue;

			const Voxel* v = chunks->getVoxel(pos);
			byte light = chunks->getChannelLight(pos, channel);
			if (light > entry.light + 1) addqueue.push(LightUint8(pos,light));
			else if ((VoxPack::isOpaque(*v) == 0)&& (light + 1 < entry.light)) {
					LightUint8 outer(pos, entry.light - 1);
					addqueue.push(outer);
					chunk->setLightGlobal(outer, channel);
			}
			
		}
	}
}
