#include "ChunkMeshQueue.h"
#include "ChunkHandle.h"

//bool ChunkMeshQueue::checkCur(SortableChunks& sBuff) {
//	if (sBuff[0].ch->flag.isInitLight() == 0) {
//		notify(_obs_event::initChunkLight, sBuff[0].ch);
//		notify(_obs_event::solveLight, 0);
//		return 1;
//	}
//	if (sBuff[0].ch->flag.isModified()) {
//		sBuff[0].ch->buildMesh();
//		return 1;
//	}
//	sBuff[0].ch->buildSortMesh(target->viewPos_);
//	return 0;
//}

void ChunkMeshQueue::step() {
	SortableChunks& sBuff = target->ch_sort;
	//if (checkCur(sBuff))return;
	/*if (sBuff[0].ch->flag.isInitLight() == 0) {
		notify(_obs_event::initChunkLight, sBuff[0].ch);
		notify(_obs_event::solveLight, 0);
		return;
	}*/
	for (size_t i = 0; i < sBuff.size(); i++) {
		if (sBuff[i].ch->flag.isInitLight() == 0) {
			notify(_obs_event::initChunkLight, sBuff[i].ch);
			notify(_obs_event::solveLight, 0);
			return;
		}
		if (sBuff[i].ch->flag.isModified()) {
			notify(_obs_event::solveQueueLight, 0);
			sBuff[i].ch->buildMesh();
			return;
		}
		sBuff[i].ch->buildSortMesh(target->viewPos_);
	}
}
ChunkMeshQueue cProcess::queue;