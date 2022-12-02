#ifndef CHUNK_MESH_QUEUE_H
#define CHUNK_MESH_QUEUE_H

#include"Game/Voxels/Chunk.h"
#include <queue>

template <typename object, typename key>
class SortableShell {
public:
	SortableShell(object* obj_, key value_) :
		obj(obj_), value(value_)
	{}
	SortableShell() {}

	key value;
	object* obj;

	inline bool operator > (const SortableShell& str) const
	{
		return (value > str.value);
	}

	inline bool operator < (const SortableShell& str) const
	{
		return (value < str.value);
	}

private:
};


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

		if (isModified || messages.size()) {
			isModified = 0;
			sort();
		}
		if(sBuffer.size()) step();
	}

	static void clear() { needClear = 1; }

private:

	static void sort() {
		if (needClear) {
			needClear = 0;
			sBuffer.clear();
		}

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

	static bool needClear;
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

