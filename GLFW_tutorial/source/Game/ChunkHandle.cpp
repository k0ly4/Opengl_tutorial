#include "ChunkHandle.h"

void Region::fill(std::vector<Chunk*>& target, size_t size, const glm::ivec2& center) {
	target.resize(size * size);
	for (size_t z = center.y; z < center.y + size; z++) {
		for (size_t x = center.x; x < center.x + size; x++) {
			target[toInt(glm::ivec2(x, z) - center, size)] = &buffer[toInt(x, z, region_size)];
		}
	}
}

void Region::generate(std::vector<Chunk>& chunks, const glm::ivec2& center) {
	chunks.resize(region_size * region_size);
	for (size_t z = 0, index = 0; z < region_size; z++) {
		for (size_t x = 0; x < region_size; x++, index++) {
			chunks[index].generate(glm::ivec3(center.x+x, 0, center.y + z));
		}
	}
}

bool Region::load(std::vector<Chunk>& empty, const glm::ivec2& center) {
	Reader reader;
	if (reader.open(getPath(center)) == 0)return 0;
	empty.resize(region_size* region_size);
	for (size_t i = 0; i < empty.size(); i++) {
		reader.read(empty[i].getVoxels());
		empty[i].setPosition(glm::ivec3(center.x + i%region_size, 0, center.y + i/region_size));
	}
	reader.close();
	return 1;
}

bool Region::save(const std::string& path, const std::vector<Chunk>& buffer) {
	Writer writer;

	if (writer.open(path) == 0) {
		LOG("Failed to save region:%s\n", path.c_str());
		return 0;
	}
	for (size_t i = 0; i < buffer.size(); i++) {
		writer.write(buffer[i].getVoxels());
	}

	writer.close();
	return 1;
}

const std::string Region::DIRECTORY = "saves\\";
const std::string Region::EXTENSION = ".region";

///ChunkHandle---------------------------------------------
/// <summary>
/// 
/// </summary>
void ChunkHandle::create(size_t size) {
	size_ = size;
	center = glm::ivec2(0);
	region.fill(chunks_,size_, center);
}

void ChunkHandle::save()const {
	LOG(LogInfo, "Region::save()\n");
	region.save();
}

const Voxel* ChunkHandle::getVoxel(const glm::ivec3& coord) {
	Chunk* chunk = get(toLocal(coord));
	if (chunk == 0)return 0;
	return chunk->getFromGlobalCoord(coord);
}

void ChunkHandle::setVoxel(const Voxel& voxel, const glm::ivec3& coord) {

	Chunk* chunk = get(toLocal(coord));
	if (chunk == 0 || chunk->getFromGlobalCoord(coord) == 0)return;
	chunk->setVoxel(voxel, coord);
}

Chunk* ChunkHandle::get(const glm::ivec3& coord) {
	
	if (isIn(coord) == 0) return 0;
	return chunks_[getIndex(coord, glm::ivec3(size_,1,size_))];

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

unsigned char ChunkHandle::getChannelLight(const glm::ivec3& coord, int channel) {
	auto local = toLocal(coord);
	if (isIn(local) == 0) return 0;
	return chunks_[toInt(local.x,local.z,size_)]->lightMap.get(coord - local*CHUNK_VOLUME, channel);
}