#ifndef CHUNK_MESH_QUEUE_H
#define CHUNK_MESH_QUEUE_H

#include"Game/Voxels/Chunk.h"
#include <queue>

///GeneratorChunkMesh---------------------------------------------
/// <summary>
/// 
/// </summary>
class ChunkMeshQueue {

public:
	typedef SortableShell<Chunk, size_t> ShellChunk;
	
	inline static void addQueue(Chunk* chunk) { messages.push(chunk);}

	static void setChunkCamera(const glm::uvec2& pos) {
		camera_pos = pos; isModified = 1;
	}

	static void stepSolveChunkMesh() {
		if (needClear) {
			messages = std::queue<Chunk*>();
			needClear = 0;
			sBuffer.clear();			
		}
		else if (isModified || messages.size()) {
			isModified = 0;
			sort();
		}
		if(sBuffer.size()) step();
	}
	inline static void waitForClear() {
		needClear = 1;
		while (isEmpty()==0)
		{
			std::this_thread::yield();
		}

	}

	inline static bool isEmpty() {return messages.empty();}

	inline static void waitForEmpty() {
		while (isEmpty() == 0)std::this_thread::yield();
		needClear = 0;
	}
	static bool needClear;
private:

	static void sort() {
		for (; messages.size();) {
 			Chunk* chunk = messages.front();
			sBuffer.push_back(ShellChunk(
				chunk,
				length(chunk->getLocalPos())
			));
			messages.pop();
		}

		std::sort(sBuffer.begin(), sBuffer.end(),
			[](const ShellChunk& lhs, const ShellChunk& rhs)
			{
				return lhs < rhs;
			});
	}

	inline static size_t length(glm::uvec2 target) {
		size_t x = std::abs((int)camera_pos.x - (int)target.x);
		size_t y = std::abs((int)camera_pos.y - (int)target.y);
		return x * x + y * y;
	}

	static void step() {
		for (size_t i = 0; i < sBuffer.size(); i++) {
			if (sBuffer[i].obj->isModified()) {
				sBuffer[i].obj->buildMesh();
				return;
			}
		}
	}

	
	static bool isModified;

	static std::queue<Chunk*> messages;
	static std::vector< ShellChunk> sBuffer;

	static glm::uvec2 camera_pos;

	ChunkMeshQueue() {}
	~ChunkMeshQueue() {}
};

class ChunkSectorRender;

class LightQueue {
public:
	static void addTask(ChunkSectorRender* chunks) {
		target = chunks;
		needUpLight = 1;
	}
	static void solve();
private:
	static bool needUpLight;
	static ChunkSectorRender* target;

	LightQueue() {}
	~LightQueue() {}
};


#endif

