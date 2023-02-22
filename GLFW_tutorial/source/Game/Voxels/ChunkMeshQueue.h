#ifndef CHUNK_MESH_QUEUE_H
#define CHUNK_MESH_QUEUE_H

#include "Game/Entities/Observer.h"
#include"Game/Voxels/Chunk.h"
#include <queue>
#include "System/Thread.h"
#include "Game/Voxels/SuperRegion.h"
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
	struct Message {
		glm::uvec2 pos;
		SortableChunks chunks;
		Message() {}
		Message(const glm::uvec2& pos_,const SortableChunks& chunks_):pos(pos_),chunks(chunks_) {}
	};
	struct ChunkShell {
		Chunk* obj;
		size_t dist;
		ChunkShell(Chunk* _chunk, size_t _distance) :obj(_chunk), dist(_distance) {}
		ChunkShell() {}
	};

public:

	enum  eSync :byte
	{
		eWork,eSwitch,eReady1, eWait,eLoad
	};

	void init(ChunkSectorRender* sector_,SupReg* region_) {
		region = region_;
		sector = sector_;

	}
	inline void addMessgae(const glm::uvec2& pos, const SortableChunks& chunks) {
		messages.push(Message(pos, chunks));
		status = eWork;
	}
	
	inline void threadGeneral() {
		if (status == eWork) {
			if (messages.empty() == 0)solveMessages();
			notify(_obs_event::solveQueueLight, 0);
			calcGeneral();
		}
		else if (status == eSwitch) status = eWait;
		else std::this_thread::yield();
	}

	inline void threadLighter() {
		if (status == eWork) {
			notify(_obs_event::solveQueueLight, 0);
			calcLight();
		}
		else if (status == eLoad) {
			while (status == eLoad) { std::this_thread::yield(); }
		}
		else __sync();
	}

	inline void threadMesher() {
		if (status == eWork) {
			if (messages.empty() == 0)	solveMessages();
			calcMesh();
		}
		else __sync();
	}

	inline void sync() {
		status = eSwitch;
		while (status != eWait)std::this_thread::yield();
	}

private:

	


	inline void __sync() {
		if (status == eSwitch) {
			status++;
			while (status == eReady1) { std::this_thread::yield(); }
		}
		else if (status == eReady1) status = eWait;
		else std::this_thread::yield();
	}

	inline void solveMessages() {
		while (messages.size() > 1) messages.pop();
		sortCh = messages.front().chunks;
		posCh = messages.front().pos;
		messages.pop();
	}

	void calcLight();
	void calcMesh();
	void calcGeneral();

	std::atomic<byte> status = eWait;

	ChunkSectorRender* sector;
	SupReg* region;

	std::queue<Message> messages;
	SortableChunks sortCh;
	glm::uvec2 posCh;
};

class cProcess {
public:

	static ChunkMeshQueue queue;

private:
	cProcess() {}
	~cProcess() {}
};

#endif

