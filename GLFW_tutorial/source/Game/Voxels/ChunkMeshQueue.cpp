#include "ChunkMeshQueue.h"
#include "ChunkHandle.h"
#include "System/Clock.h"


void ChunkMeshQueue::calcLight() {
	for (size_t i = 0; i < sortCh.size(); i++) {
		if (sortCh[i].ch->flag.isDraw == 0)continue;

		if (sortCh[i].ch->flag.status == StateChunk::s_lighting_phase_adding) {
			LogClock log;
			notify(_obs_event::solveQueueLight, 0);
			notify(_obs_event::initChunkLight, sortCh[i].ch);
			notify(_obs_event::solveLight, 0);
			log.logIf_sec(0.9f, "Light");
			return;
		}
		
	}
}

void ChunkMeshQueue::calcMesh() {
	for (size_t i = 0; i < sortCh.size(); i++) {

		if (sortCh[i].ch->flag.isDraw == 0)continue;

		if (sortCh[i].ch->flag.status == StateChunk::s_mesh) {
			for (int j = 4; j > -1; j--) {
				if (sortCh[j].ch->flag.status == StateChunk::s_mesh) {
					sortCh[i].ch->buildMesh();
					sortCh[j].ch->buildSortMesh(sector->viewPos_);
					return;
				}
			}
			sortCh[i].ch->buildMesh();
			sortCh[i].ch->buildSortMesh(sector->viewPos_);
			return;
		}
		sortCh[i].ch->buildSortMesh(sector->viewPos_);
	}
}

void ChunkMeshQueue::calcGeneral() {
	for (size_t i = 0; i < sortCh.size(); i++) {
		if (sortCh[i].ch->flag.isDraw == 0)continue;

		if (sortCh[i].ch->flag.status == StateChunk::s_lighting_phase_adding) {
			LogClock log;
			notify(_obs_event::initChunkLight, sortCh[i].ch);
			notify(_obs_event::solveLight, 0);
			log.logIf_sec(0.9f,"Light");
			return;
		}
		if (sortCh[i].ch->flag.status == StateChunk::s_mesh) {
			
			for (int j = 4; j >-1; j--) {
				if (sortCh[j].ch->flag.status == StateChunk::s_mesh) {
					notify(_obs_event::solveQueueLight, 0);
					sortCh[j].ch->buildMesh();
					sortCh[j].ch->buildSortMesh(sector->viewPos_);
					return;
				}
			}
			
			notify(_obs_event::solveQueueLight, 0);
			sortCh[i].ch->buildMesh();
			sortCh[i].ch->buildSortMesh(sector->viewPos_);
			return;
		}
		sortCh[i].ch->buildSortMesh(sector->viewPos_);
	}

	for (size_t i = 0; i < sortCh.size(); i++) {

		if (sortCh[i].ch->flag.status == StateChunk::s_lighting_phase_adding) {
			LogClock log;
			notify(_obs_event::initChunkLight, sortCh[i].ch);
			notify(_obs_event::solveLight, 0);
			log.logIf_sec(0.9f, "Light");
			return;
		}

		if (sortCh[i].ch->flag.status == StateChunk::s_mesh) {
			for (int j = 4; j > -1; j--) {
				if (sortCh[j].ch->flag.status == StateChunk::s_mesh) {
					notify(_obs_event::solveQueueLight, 0);
					sortCh[j].ch->buildMesh();
					sortCh[j].ch->buildSortMesh(sector->viewPos_);
					return;
				}
			}

			notify(_obs_event::solveQueueLight, 0);
			sortCh[i].ch->buildMesh();
			sortCh[i].ch->buildSortMesh(sector->viewPos_);

			return;
		}
		sortCh[i].ch->buildSortMesh(sector->viewPos_);
	}
}
ChunkMeshQueue cProcess::queue;