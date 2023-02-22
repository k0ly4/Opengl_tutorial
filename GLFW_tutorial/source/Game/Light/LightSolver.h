#ifndef LIGHT_SOLVER_h
#define LIGHT_SOLVER_h
#include "Game/Voxels/ChunkHandle.h"
#include <queue>
#include <map>
class LightQueue {
public:
	LightQueue() :posSupRegVox(0) {}
	inline void push(const LightUint8& light) noexcept{
		size_t index = toInt(light.pos - posSupRegVox , glm::uvec3(SUPREG_SIZE_VOX,CHUNK_H, SUPREG_SIZE_VOX));
		auto i = buffer.find(index);
		;
		if (i != buffer.end()) {
			if (light.light > i->second.light) i->second = light;
		}
		else buffer.insert({ index,light});
	}
	inline LightUint8 front()	noexcept { return buffer.begin()->second; }
	inline void pop()			noexcept { buffer.erase(buffer.begin());}
	inline bool empty()			noexcept { return buffer.empty(); }

	inline void setPositionSuperRegionCh(glm::uvec2 pos) { posSupRegVox = glm::uvec3(pos.x*CHUNK_W,0,pos.y*CHUNK_D); }

private:

	glm::uvec3 posSupRegVox;
	std::map<size_t, LightUint8> buffer;

};

class LightSolver {

public:

	LightSolver() {}
	LightSolver(SupReg* chunks_, int channel_) {
		init(chunks_, channel_);
	}
	inline void init(SupReg* chunks_, int channel_) {
		region = chunks_;
		channel = channel_;
	}
	inline void add(const glm::ivec3& pos){		add(pos, region->getChannelLight(pos, channel)); }
	inline void add(int x, int y, int z) {		add(glm::ivec3(x, y, z)); }

	void		add(const glm::ivec3& pos, byte emission);
	inline void add(int x, int y, int z, byte emission) { add(glm::ivec3(x, y, z), emission); }

	void remove(const glm::ivec3& pos);
	inline void remove(int x, int y, int z) { remove(glm::ivec3(x, y, z)); }

	void solve();

private:
	static const  glm::ivec3 off[];

	inline void solve_rem_rec(const LightUint8& entry)noexcept {

		for (size_t i = 0; i < 6; i++) {
			glm::uvec3 pos((glm::ivec3)entry.pos + off[i]);
			Chunk* chunk = region->getByVoxel(pos);
			if (chunk == 0) continue;

			byte light = region->getChannelLight(pos, channel);
			//Если свет не от текущего источника то добавить
			if (light >= entry.light) addqueue.push(LightUint8(pos, light));
			//Если свет от этого источника то дополнить очередь очистки
			else if ((light != 0) && (light == entry.light - 1)) {
				chunk->setLightGlobal(LightUint8(pos, 0), channel);
				solve_rem_rec(LightUint8(pos, light));
			}
		}
	}

	inline byte maxLight(const glm::ivec3& pos) {
		return	std::max({
			region->getChannelLight(pos, channel),
			region->getChannelLight(pos.x + 1,pos.y,pos.z, channel),
			region->getChannelLight(pos.x - 1,pos.y,pos.z, channel),
			region->getChannelLight(pos.x,pos.y + 1,pos.z, channel),
			region->getChannelLight(pos.x,pos.y - 1,pos.z, channel),
			region->getChannelLight(pos.x,pos.y,pos.z + 1, channel),
			region->getChannelLight(pos.x,pos.y,pos.z - 1, channel)
			});
	}
	std::queue<LightUint8> addqueue;
	std::queue<LightUint8> remqueue;
	
	///LightQueue qtmp;
	std::queue<LightUint8> qtmp;
	SupReg* region;
	int channel;

};
#endif
