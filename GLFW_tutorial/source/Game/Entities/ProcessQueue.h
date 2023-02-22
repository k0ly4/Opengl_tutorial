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
		//process = new std::thread(&ProcessQueue::update, this);
		main.launch(this,	&ProcessQueue::update);
	}

	inline void kill() {
		life = 0;
		main.kill();
	}

	~ProcessQueue() {
		kill();
	}
private:

	void update() {
		while (life) {
			cProcess::queue.threadGeneral();
		}
	}

	void upd1() {
		while (life) {
			cProcess::queue.threadMesher();
		}
	}
	void upd2() {
		while (life) {
			cProcess::queue.threadLighter();;
		}
	}
	Thread main;
	bool life;

};


#endif
