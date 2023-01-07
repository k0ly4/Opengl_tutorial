#ifndef LIGHT_SOLVER_h
#define LIGHT_SOLVER_h
#include "Game/Voxels/ChunkHandle.h"
#include <queue>


class LightSolver {

public:

	LightSolver() {}
	LightSolver(ChunkSectorRender* chunks_, int channel_) {
		init(chunks_, channel_);
	}
	inline void init(ChunkSectorRender* chunks_, int channel_) {
		chunks = chunks_;
		channel = channel_;
	}
	inline void add(const glm::ivec3& pos){		add(pos, chunks->getChannelLight(pos, channel)); }
	inline void add(int x, int y, int z) {		add(glm::ivec3(x, y, z)); }

	void		add(const glm::ivec3& pos, byte emission);
	inline void add(int x, int y, int z, byte emission) { add(glm::ivec3(x, y, z), emission); }

	void remove(const glm::ivec3& pos);
	inline void remove(int x, int y, int z) { remove(glm::ivec3(x, y, z)); }

	void solve();

private:
	inline byte maxLight(const glm::ivec3& pos) {
		return	std::max({
			chunks->getChannelLight(pos, channel),
			chunks->getChannelLight(pos.x + 1,pos.y,pos.z, channel),
			chunks->getChannelLight(pos.x - 1,pos.y,pos.z, channel),
			chunks->getChannelLight(pos.x,pos.y + 1,pos.z, channel),
			chunks->getChannelLight(pos.x,pos.y - 1,pos.z, channel),
			chunks->getChannelLight(pos.x,pos.y,pos.z + 1, channel),
			chunks->getChannelLight(pos.x,pos.y,pos.z - 1, channel)
			});
	}
	std::queue<LightUint8> addqueue;
	std::queue<LightUint8> remqueue;
	ChunkSectorRender* chunks;
	int channel;

};
#endif
