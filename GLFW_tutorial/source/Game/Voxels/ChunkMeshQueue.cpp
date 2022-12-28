#include "ChunkMeshQueue.h"
#include "ChunkHandle.h"

void ChunkMeshQueue::step() {
	size_t factMod = 1;
	SortableChunks& sBuff = target->ch_sort;
	for (size_t i = 0; i < sBuff.size(); i++) {
		if (sBuff[i].ch->flag.isInitLight() == 0) {
			notify(_obs_event::initChunkLight, sBuff[i].ch);
			notify(_obs_event::solveLight, 0);
		}
		if (sBuff[i].ch->flag.isModified()) {
			sBuff[i].ch->buildMesh();
			if (--factMod == 0)return;
		}
		sBuff[i].ch->buildSortMesh(target->viewPos_);
	}
}
ChunkMeshQueue cProcess::queue;