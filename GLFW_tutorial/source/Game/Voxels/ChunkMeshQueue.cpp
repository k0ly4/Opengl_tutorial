#include "ChunkMeshQueue.h"
#include "ChunkHandle.h"

void ChunkMeshQueue::step() {
	SortableChunks& sBuff = target->ch_sort;

	for (size_t i = 0; i < sBuff.size(); i++) {
		if (sBuff[i].ch->flag.isInitLight() == 0) {
			notify(_obs_event::initChunkLight, sBuff[i].ch);
			notify(_obs_event::solveLight, 0);
			return;
		}
		if (sBuff[i].ch->flag.isModified()) {
			if (i > 3) {
				for (size_t j = 0; j < 4; j++) {
					if (sBuff[j].ch->flag.isModified()) {
						notify(_obs_event::solveQueueLight, 0);
						sBuff[j].ch->buildMesh();
						return;
					}
				}
			}
			notify(_obs_event::solveQueueLight, 0);
			sBuff[i].ch->buildMesh();
			return;
		}
		sBuff[i].ch->buildSortMesh(target->viewPos_);
	}
}
ChunkMeshQueue cProcess::queue;