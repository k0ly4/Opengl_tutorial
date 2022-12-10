#include "ChunkHandle.h"

///ChunkHandle---------------------------------------------
/// <summary>
/// 
/// </summary>
void ChunkSectorRender::setSize(size_t size) {
	size_ = size;
	__chunks_.resize(size_ * size_);
	extractFromRegion();
}

void ChunkSectorRender::extractFromRegion() {

	ChunkMeshQueue::needClear =1;
	begin_ = cameraChunk_- size_/2;
	ChunkMeshQueue::waitForEmpty();

	region_->fillSector(__chunks_, size_, begin_);
	render_chunks = __chunks_;
	LightQueue::addTask(this);
	ChunkMeshQueue::setChunkCamera(cameraChunk_);
	for (size_t i = 0; i < render_chunks.size(); i++) ChunkMeshQueue::addQueue(render_chunks[i]);

}

void ChunkSectorRender::lightFlagUp() {
	for (size_t i = 0; i < render_chunks.size(); i++)
		if (render_chunks[i]->isInitLightMap == 0) notify(_obs_event::initChunkLight, render_chunks[i]);
	notify(_obs_event::solveLight, 0);
}

void ChunkSectorRender::setCameraPos(const glm::ivec3& positionCamera) {
	
	glm::uvec2 posChunk(positionCamera.x / CHUNK_W, positionCamera.z / CHUNK_D);
	if (cameraChunk_ == posChunk) return;
	cameraChunk_ = posChunk;
	LOG("ChunkSectorRender::x=%d,y=%d\n", cameraChunk_.x, cameraChunk_.y);
	extractFromRegion();
}

const Voxel* ChunkSectorRender::getVoxel(const glm::uvec3& coord) {
	glm::uvec3  local = toLocal(coord);
	Chunk* chunk = get(local);
	if (chunk == 0)return 0;
	return chunk->getFromGlobalCoord(coord);
}

void ChunkSectorRender::setVoxel(const Voxel& voxel, const glm::ivec3& coord) {

	Chunk* chunk = get(toLocal(coord));
	if (chunk == 0 || chunk->getFromGlobalCoord(coord) == 0)return;
	chunk->setVoxel(voxel, coord);
}

Chunk* ChunkSectorRender::get(const glm::uvec3& coord) {
	
	if (isIn(coord) == 0) return 0;
	return __chunks_[getIndex(coord, glm::uvec3(size_,1,size_))];

}

const Voxel* ChunkSectorRender::rayCast(const glm::vec3& a, const glm::vec3& dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::ivec3& iend) {
	float px = a.x;
	float py = a.y;
	float pz = a.z;

	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float t = 0.0f;
	int ix = (int)floor(px);
	int iy = (int)floor(py);
	int iz = (int)floor(pz);

	float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
	float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
	float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

	constexpr float infinity = std::numeric_limits<float>::infinity();

	float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
	float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
	float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

	float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
	float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
	float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

	float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
	float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
	float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

	int steppedIndex = -1;

	while (t <= maxDist) {
		const Voxel* voxel = getVoxel(glm::ivec3(ix, iy, iz));
		if (voxel != nullptr && VoxelPack::isSelectable(*voxel)) {
			end.x = px + t * dx;
			end.y = py + t * dy;
			end.z = pz + t * dz;

			iend.x = ix;
			iend.y = iy;
			iend.z = iz;

			norm.x = norm.y = norm.z = 0.0f;
			if (steppedIndex == 0) norm.x = -stepx;
			if (steppedIndex == 1) norm.y = -stepy;
			if (steppedIndex == 2) norm.z = -stepz;
			return voxel;
		}

		if (txMax < tyMax) {
			if (txMax < tzMax) {
				ix += (int)stepx;
				t = txMax;
				txMax += txDelta;
				steppedIndex = 0;
			}
			else {
				iz += (int)stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
		else {
			if (tyMax < tzMax) {
				iy += (int)stepy;
				t = tyMax;
				tyMax += tyDelta;
				steppedIndex = 1;
			}
			else {
				iz += (int)stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
	}
	iend.x = ix;
	iend.y = iy;
	iend.z = iz;

	end.x = px + t * dx;
	end.y = py + t * dy;
	end.z = pz + t * dz;
	norm.x = norm.y = norm.z = 0.0f;
	return nullptr;
}

unsigned char ChunkSectorRender::getChannelLight(const glm::uvec3& coord, int channel) {
	glm::uvec3 local = toLocal(coord);

	if (isIn(local) == 0) return 0;
	return __chunks_[toInt(local.x, local.z , size_)]->lightMap.get(coord - (local+ glm::uvec3(begin_.x,0,begin_.y)) * CHUNK_VOLUME, channel);
}