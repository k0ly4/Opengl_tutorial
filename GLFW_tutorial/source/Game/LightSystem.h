#ifndef LIGHT_HANDLE_H
#define LIGHT_HANDLE_H

#include "Game/LightSolver.h"

class LightHandle
{
public:

	LightHandle(ChunkHandle *chunks_):
		chunks(chunks_)
	{
			solverR.init(chunks,0);
			solverG.init(chunks,1);
			solverB.init(chunks,2);
			solverS.init(chunks,3);
	}

	void init();
	void add(const glm::ivec3& pos, Voxel voxel);
	
	void remove(const glm::ivec3& pos);
private:

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

	ChunkHandle* chunks;
	LightSolver  solverR;
	LightSolver  solverG;
	LightSolver  solverB;
	LightSolver  solverS;
};

#endif
