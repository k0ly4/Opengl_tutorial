#ifndef PROCESS_QUEUE_H
#define PROCESS_QUEUE_H
#include"Game/Voxels/ChunkMeshQueue.h"

///ProcessQueue---------------------------------------------
/// <summary>
/// 
/// </summary>
/// 
class ProcessQueue {
public:

	ProcessQueue() { life = 0; }

	void launch() {
		life = 1;
		process = new std::thread(&ProcessQueue::update, this);
	}

	inline void kill() {
		life = 0;
		process->join();
		delete process;
		process = 0;
	}

	~ProcessQueue() {
		if (process)kill();
	}
private:

	void update() {
		while (life) {
			cProcess::queue.stepSolveChunkMesh();
		}
	}

	std::thread* process = 0;
	bool life;

};


#endif
