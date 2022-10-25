#include "ChunkHandle.h"

///ChunkHandle---------------------------------------------
/// <summary>
/// 
/// </summary>
void ChunkHandle::create(const glm::uvec3& volume) {
	volume_ = volume;
	chunks_.resize(volume_.x * volume_.y * volume_.z);
	size_t index = 0;
	for (size_t y = 0; y < volume_.y; y++) {
		for (size_t z = 0; z < volume_.z; z++) {
			for (size_t x = 0; x < volume_.x; x++) {
				if (chunks_[index].load(CHUNK_SIZE, glm::ivec3(x, y, z)) == 0) {
					chunks_[index].generate(CHUNK_SIZE, glm::ivec3(x, y, z));
					LOG("Not succes load chunk\n");
				}
				index++;
			}
		}
	}
	for (size_t i = 0; i < chunks_.size(); i++)
		chunks_[i].setCloses(chunks_);
}

void ChunkHandle::save()const {
	LOG(LogInfo, "ChunkHandle::save()\n");
	for(size_t i=0;i<chunks_.size();i++)
	{
		chunks_[i].save();
	}
}

const Voxel* ChunkHandle::getVoxel(const glm::ivec3& coord) {

	auto local = coord / (int)CHUNK_SIZE;
	if (coord.x < 0) local.x--;
	if (coord.y < 0) local.y--;
	if (coord.z < 0) local.z--;

	Chunk* chunk = get(local);
	if (chunk == 0)return 0;
	return chunk->getGlobal(coord);
}

void ChunkHandle::setVoxel(const Voxel& voxel, const glm::ivec3& coord) {

	auto local = coord / (int)CHUNK_SIZE;
	if (coord.x < 0) local.x--;
	if (coord.y < 0) local.y--;
	if (coord.z < 0) local.z--;

	Chunk* chunk = get(local);
	if (chunk == 0 || chunk->getGlobal(coord) == 0)return;
	chunk->setVoxel(voxel, coord);
}

Chunk* ChunkHandle::get(const glm::ivec3& coord) {
	if (coord.x < 0 || coord.z < 0 || coord.y < 0 || coord.x >= volume_.x || coord.y >= volume_.y || coord.z >= volume_.z) return 0;
	return &chunks_[getIndex(coord, volume_)];

}

bool ChunkHandle::setVoxel(const Voxel& voxel, const glm::vec3& start, const glm::vec3& dir, float dist, bool adMode) {
	glm::vec3 end;
	glm::vec3 norm;
	glm::ivec3 iend;
	const Voxel* voxel_ = rayCast(start, dir, dist, end, norm, iend);
	if (voxel_ == 0)
		return 0;
	if (adMode == 1) {
		setVoxel(voxel, iend + glm::ivec3(norm));
		LOG("AdMode::x:%d,y:%d,z:%d;norm:x:%f,y:%f,z:%f\n", iend.x, iend.y, iend.z, norm.x, norm.y, norm.z);
	}
	else {
		setVoxel(voxel, iend);
		LOG("DecMode::x:%d,y:%d,z:%d;norm:x:%f,y:%f,z:%f\n", iend.x, iend.y, iend.z, norm.x, norm.y, norm.z);
	}
	return 1;
}

const Voxel* ChunkHandle::rayCast(const glm::vec3& a, const glm::vec3& dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::ivec3& iend) {
	float px = a.x;
	float py = a.y;
	float pz = a.z;

	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float t = 0.0f;
	int ix = floor(px);
	int iy = floor(py);
	int iz = floor(pz);

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
		if (voxel != nullptr && isValid(*voxel)) {
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
				ix += stepx;
				t = txMax;
				txMax += txDelta;
				steppedIndex = 0;
			}
			else {
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
		else {
			if (tyMax < tzMax) {
				iy += stepy;
				t = tyMax;
				tyMax += tyDelta;
				steppedIndex = 1;
			}
			else {
				iz += stepz;
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