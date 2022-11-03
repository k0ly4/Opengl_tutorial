#ifndef LIGHT_SOLVER_h
#define LIGHT_SOLVER_h
#include "Game/ChunkHandle.h"
#include <queue>


class LightSolver {
	
public:

	LightSolver() {}
	LightSolver(ChunkHandle* chunks_, int channel_) {
		init(chunks_, channel_);
	}
	void init(ChunkHandle* chunks_, int channel_) {
		chunks = chunks_;
		channel = channel_;
	}
	void add(const glm::ivec3& pos);
	inline void add(int x, int y, int z) { add(glm::ivec3(x, y, z)); }
	void add(const glm::ivec3& pos, int emission);
	inline void add(int x, int y, int z, int emission) { add(glm::ivec3(x, y, z), emission); }
	void remove(const glm::ivec3& pos);
	inline void remove(int x, int y, int z) { remove(glm::ivec3(x, y, z)); }
	void solve();

private:
	
	std::queue<LightUint8> addqueue;
	std::queue<LightUint8> remqueue;
	ChunkHandle* chunks;
	int channel;

};
#endif
