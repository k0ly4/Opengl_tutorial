#ifndef LIGHT_HANDLE_H
#define LIGHT_HANDLE_H

#include "Game/Light/LightSolver.h"

class LightHandle :public uniObserver<Chunk>
{
public:
	struct Mods {
		glm::ivec3 pos;
		Voxel vox;
	};
	LightHandle(SupReg* chunks_) :
		region(chunks_)
	{
		solverR.init(region,0);
		solverG.init(region,1);
		solverB.init(region,2);
		solverS.init(region,3);

	}

	void chunkInit(Chunk& chunk);
	void addToQueue(const glm::ivec3& pos, Voxel voxel) { queue.push({ pos,voxel });}

private:

	inline void solveQueue() {
		while (queue.size()) {
			Mods cur(queue.front());
			queue.pop();
			if (VoxPack::isTransparent(cur.vox))
			{
				if (VoxPack::isTransparent(*region->getVoxel(cur.pos))) remove(cur.pos);
			}
			else	add(cur.pos, cur.vox);
		}
	}
	//Для эмиссионных и непрозрачных
	void add(const glm::ivec3& pos, Voxel voxel);
	//Для прозрачных
	void remove(const glm::ivec3& pos);

	inline void handleEvent(int id, Chunk* value) {
		if (id ==			_obs_event::initChunkLight) chunkInit(*value);
		else if (id ==		_obs_event::solveLight) solveRGBS();
		else if(id==		_obs_event::solveQueueLight)solveQueue();
	}

	inline void removeRGB(const glm::ivec3& pos) {
		solverR.remove(pos);
		solverG.remove(pos);
		solverB.remove(pos);
	}

	inline void removeRGBS(const glm::ivec3& pos) {
		removeRGB(pos);
		solverS.remove(pos);
	}

	inline void addRGB(int x, int y, int z) {
		solverR.add(x, y, z); 
		solverG.add(x, y, z); 
		solverB.add(x, y, z); 
	}

	inline void addRGB(int x, int y, int z, int light) {
		solverR.add(x, y, z, light);
		solverG.add(x, y, z, light);
		solverB.add(x, y, z, light);
	}

	inline void addRGB(const glm::ivec3& pos, byte light) {
		solverR.add(pos, light);
		solverG.add(pos, light);
		solverB.add(pos, light);
	}

	inline void addRGB(const glm::ivec3& pos, Uint8RGB light) {
		solverR.add(pos, light.r);
		solverG.add(pos, light.g);
		solverB.add(pos, light.b);
	}

	inline void addRGBS(int x, int y, int z) {
		addRGB(x, y, z);
		solverS.add(x, y, z);
	}

	inline void solveRGB() {
		solverR.solve();
		solverG.solve();
		solverB.solve();
	}

	inline void solveRGBS() {
		solveRGB();
		solverS.solve();
	}
	std::queue<Mods> queue;
	SupReg* region;
	//ChunkSectorRender* chunks;

	LightSolver solverR;
	LightSolver solverG;
	LightSolver solverB;
	LightSolver solverS;

};

#endif
