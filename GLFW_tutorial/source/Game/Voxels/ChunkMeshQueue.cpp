#include "ChunkMeshQueue.h"
#include "ChunkHandle.h"

void ChunkMeshQueue::step() {
	SortableChunks& sBuff = target->ch_sort;

	for (size_t i = 0; i < sBuff.size(); i++) {
		if (sBuff[i].ch->flag.isDraw == 0)continue;
		if (sBuff[i].ch->flag.isInitLight() == 0) {
			notify(_obs_event::initChunkLight, sBuff[i].ch);
			notify(_obs_event::solveLight, 0);
			return;
		}
		if (sBuff[i].ch->flag.isModified()) {
			
			for (int j = 4; j >-1; j--) {
				if (sBuff[j].ch->flag.isModified()) {
					notify(_obs_event::solveQueueLight, 0);
					sBuff[j].ch->buildMesh();
					sBuff[j].ch->buildSortMesh(target->viewPos_);
					return;
				}
			}
			
			notify(_obs_event::solveQueueLight, 0);
			sBuff[i].ch->buildMesh();
			sBuff[i].ch->buildSortMesh(target->viewPos_);
			return;
		}
		sBuff[i].ch->buildSortMesh(target->viewPos_);
	}
}
ChunkMeshQueue cProcess::queue;