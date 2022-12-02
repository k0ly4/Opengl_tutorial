#include "ChunkMeshQueue.h"
#include "ChunkHandle.h"
bool ChunkMeshQueue::needClear =0;
bool ChunkMeshQueue::isModified =0;

std::queue<Chunk*> ChunkMeshQueue::messages;
std::vector<ChunkMeshQueue::ShellChunk> ChunkMeshQueue::sBuffer;

glm::uvec2 ChunkMeshQueue::camera_pos(0.f);


void LightQueue::solve() {
	if (needUpLight) {
		needUpLight = 0;
		target->lightFlagUp();
	}
}
 bool LightQueue::needUpLight =0;
ChunkSectorRender* LightQueue::target =0;