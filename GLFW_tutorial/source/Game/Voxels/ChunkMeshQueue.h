#ifndef CHUNK_MESH_QUEUE_H
#define CHUNK_MESH_QUEUE_H

#include "Game/Entities/Observer.h"
#include"Game/Voxels/Chunk.h"
#include <queue>
class ChunkSectorRender;
struct SortChunk {
	size_t d;
	Chunk* ch;
};
typedef std::vector<SortChunk> SortableChunks;
///GeneratorChunkMesh---------------------------------------------
/// <summary>
/// 
/// </summary>
class ChunkMeshQueue : public uniListener<Chunk> {

	struct ChunkShell {
		Chunk* obj;
		size_t dist;
		ChunkShell(Chunk* _chunk, size_t _distance) :obj(_chunk), dist(_distance) {}
		ChunkShell() {}
	};

public:

	enum  eSync :byte
	{
		eWait, eWork,eSwitch,
	};

	inline void addToQueue(ChunkSectorRender* chunk) {
		target = chunk;
		isSync = eWork;
	}

	inline void stepSolveChunkMesh() {
		if (isSync == eWork) {
			notify(_obs_event::solveQueueLight, 0);
			step();
		}
		else if (isSync == eSwitch) isSync = eWait; 
		else std::this_thread::yield();
	}
	inline void sync() {
		isSync = eSwitch;
		while (isSync == eSwitch)std::this_thread::yield();
	}

private:
	//bool checkCur(SortableChunks& sBuff);
	void step();
	byte isSync = eWait;
	ChunkSectorRender* target;

};
class cProcess {
public:

	static ChunkMeshQueue queue;

private:
	cProcess() {}
	~cProcess() {}
};

#endif

