#include "ChunkHandle.h"

///ChunkHandle---------------------------------------------
/// <summary>
/// 
/// </summary>
void ChunkSectorRender::setSize(size_t size) {
	size_ = size;
	chunks_.setSize(size_);
	extractFromRegion();
}

void ChunkSectorRender::extractFromRegion() {
	cProcess::queue.sync();
	begin_ = viewCh_ - size_ / 2;
	region_->fillSector(chunks_, size_, begin_);
	upChunks_sort();
	cProcess::queue.addToQueue(this);
}

void ChunkSectorRender::setCameraPos(const glm::ivec3& posView) {
	glm::uvec2 posViewCh(posView.x / CHUNK_W, posView.z / CHUNK_D);
	if (posViewCh == viewCh_) return;
	viewPos_ = posView;
	viewCh_ = posViewCh;
	LOG("ChunkSectorRender::x=%d,y=%d\n", viewCh_.x, viewCh_.y);
	extractFromRegion();
}

void ChunkSectorRender::setVoxel(const Voxel& voxel, const glm::ivec3& coord) {
	Chunk* chunk = getByVoxel(coord);
	if (chunk) chunk->setVoxel(voxel, coord);
}
inline size_t length(const glm::uvec2& p1,const glm::uvec3& p2) {
	size_t x = std::abs((int)p1.x - (int)p2.x);
	size_t y = std::abs((int)p1.y - (int)p2.z);
	return x * x + y * y;
}
void ChunkSectorRender::upChunks_sort() {
	SortableChunks ch(chunks_.size());
	for (size_t i = 0; i < ch.size(); i++) {
		ch[i].ch = chunks_[i];
		ch[i].d = length(viewCh_, ch[i].ch->localPos());
	}
	std::sort(ch.begin(), ch.end(), [](const SortChunk& l, const SortChunk& r) {return r.d > l.d;});
	ch_sort=ch;
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