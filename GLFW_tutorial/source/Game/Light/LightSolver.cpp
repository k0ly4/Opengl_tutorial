#include "LightSolver.h"
//Добавляем эмиссию(emission > 1)
void LightSolver::add(const glm::ivec3& pos, byte emission) {
	if (emission < 2) return;
	Chunk* chunk = region->getByVoxel(pos);
	LightUint8 entry(pos, emission);
	addqueue.push(entry);
	chunk->setLightGlobal(entry, channel);
}
//Обнуляем
void LightSolver::remove(const glm::ivec3& pos) {
	Chunk* chunk = region->getByVoxel(pos.x, pos.y,pos.z);
	byte light = chunk->getLightGlobal(pos, channel);
	//if (light == 0) return;
	remqueue.push(LightUint8(pos, light));
	chunk->setLightGlobal(LightUint8(pos,0), channel);
}

void LightSolver::solve() {

	while (!remqueue.empty()) {
		//solve_rem_rec(remqueue.front());
		//remqueue.pop();
		LightUint8 entry(remqueue.front());
		remqueue.pop();
		for (size_t i = 0; i < 6; i++) {
			glm::uvec3 pos(glm::ivec3(entry.pos) + off[i]);
			Chunk* chunk = region->getByVoxel(pos);
			if (chunk == 0 || (chunk->flag.status < StateChunk::s_lighting_phase_solving)) continue;

			byte light = region->getChannelLight(pos, channel);
			//Если свет не от текущего источника то добавить
			if (light >= entry.light) addqueue.push(LightUint8(pos,light));
			//Если свет от этого источника то дополнить очередь очистки
			else if ((light != 0) && (light == entry.light - 1)) {
				remqueue.push(LightUint8(pos, light));
				chunk->setLightGlobal(LightUint8(pos, 0), channel);
			}

		}
	}
	//qtmp.setPositionSuperRegionCh(region->posCh())
	while (!addqueue.empty()) {
		LightUint8 entry(addqueue.front());
		addqueue.pop();
		if (entry.light == 0) continue;

		for (size_t i = 0; i < 6; i++) {
			glm::uvec3 pos(glm::ivec3(entry.pos) + off[i]);
			Chunk* chunk = region->getByVoxel(pos);
			if (chunk == 0) continue;

			Voxel v = *chunk->getGlobal(pos);
			byte light = chunk->getLightGlobal(pos, channel);
			//если текущий воксель не был источником света
			if (light > entry.light + 1) {
				qtmp.push(LightUint8(pos, light));
				break;
			}
			//если текущий воксель источник света и нет препятствий
			else if (VoxPack::isTransparent(v) && (light + 1 < entry.light)) {
				LightUint8 outer(pos, entry.light - 1);
				qtmp.push(outer);
				chunk->setLightGlobal(outer, channel);
			}
		}
		
		while (!qtmp.empty()) {
			LightUint8 entry(qtmp.front());
			qtmp.pop();

			if (entry.light == 0 || region->getChannelLight(entry.pos, channel) > entry.light) continue;
			
			for (size_t i = 0; i < 6; i++) {
				glm::uvec3 pos(glm::ivec3(entry.pos) + off[i]);
				Chunk* chunk = region->getByVoxel(pos);

				if (chunk == 0 || (chunk->flag.status < StateChunk::s_lighting_phase_solving)) continue;

				glm::uvec3 local_pos (chunk->toLocalVx(pos));
				Voxel v = chunk->voxels()(local_pos);
				byte light = chunk->lightMap.get(local_pos, channel);
				//если текущий воксель не был источником света
				if (light > entry.light + 1) {
					qtmp.push(LightUint8(pos, light));
					break;
				}
				//если текущий воксель источник света и нет препятствий
				else if (VoxPack::isTransparent(v) && (light + 1 < entry.light)) {
					LightUint8 outer(pos, entry.light - 1);
					qtmp.push(outer);
					chunk->setLightGlobal(outer, channel);
				}
			}
			
		}
	}
}
const  glm::ivec3 LightSolver::off[] = {
		{0, 0, 1},
		{0, 0,-1},
		{0, 1, 0},
		{0,-1, 0},
		{1, 0, 0},
		{-1,0, 0}
};