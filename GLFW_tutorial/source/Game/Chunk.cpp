#include "Chunk.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>
//VoxelAtlas-------------------------------------
/// <summary>
/// 
/// </summary>
void VoxelAtlas::load(const std::string& path, size_t sizeVoxelUV) {
	texture_.getPath(path,4);
	texture_.filter(TextureFilter::Nearest, TextureFilter::NearestMipmapNearest);
	sizeVoxel_ = sizeVoxelUV;
	uvSize_ = (float)sizeVoxel_ / (float)texture_.getSize().x;
	
	//scan
	scan.resize(id_max);
	scan[id_turf].id[bottom] = uv_earth;
	scan[id_turf].id[front] = scan[id_turf].id[left]= scan[id_turf].id[right] = scan[id_turf].id[back] = uv_side_turf;
	scan[id_turf].id[top] = uv_turf;

	scan[id_earth].setSolid(uv_earth);
	//uv
	uv.resize(uv_max);
	for (size_t i = 0; i < uv_max; i++) {
		uv[i] = getUV(i);
	}
}


//CHunk-------------------------------------
/// <summary>
/// 
/// </summary>
void Chunk::generate(size_t size,const glm::ivec3& global) {
		voxels.resize(size* size* size);
		global_ = global* (int)size;
		size_ = size;
		for (size_t y = 0; y < size; y++) {
			for (size_t z = 0; z < size; z++) {
				for (size_t x = 0; x < size; x++)
				{
					glm::ivec3 real_coord = glm::ivec3(x,y,z) + global_;
					int id = (glm::perlin(glm::vec3(real_coord) * 0.0125f) > 0.1f)-1;//real_y <= (height) * 60 + 30;
					if (real_coord.y <= 2)
						id = 1;
					getLocal(x, y, z).id = id;
				}
			}
		}
		modified = 1;
	}

void Chunk::setCloses(const std::vector<Chunk>& chunks) {

	for (size_t i = 0; i < 6; i++) {
		closes[i] = nullptr;
	}

	for (size_t i = 0; i < chunks.size(); i++) {
		glm::ivec3 dist = (chunks[i].global_ / (int)size_ - global_ / (int)size_) ;
		if (((abs(dist.x) + abs(dist.y) + abs(dist.z)) != 1))
			continue;
		closes[getSide(dist)] = &chunks[i];
	}
}

bool Chunk::setVoxel(const Voxel& voxel, const glm::uvec3& coord) {
	Voxel* voxel_ = getGlobal(coord);
	if (voxel_ == 0) return 0;
	if (*voxel_ == voxel) return 1;

	*voxel_ = voxel;
	modified = 1;
	modifiedCloses(coord - glm::uvec3(global_));
	return 1;
}

void Chunk::modifiedCloses(const glm::uvec3& local) {

	if (local.x == size_ - 1) setModifiedCloses(right);
	else if (local.x == 0) setModifiedCloses(left);

	if (local.y == size_ - 1) setModifiedCloses(top);
	else if (local.y == 0) setModifiedCloses(bottom);

	if (local.z == size_ - 1) setModifiedCloses(front);
	else if (local.z == 0) setModifiedCloses(back);
}

inline void push_back(std::vector<unsigned>& indices,size_t begin) {
	indices.push_back(begin);
	indices.push_back(begin + 1);
	indices.push_back(begin + 2);
	indices.push_back(begin);
	indices.push_back(begin + 2);
	indices.push_back(begin + 3);
}

void Chunk::upMesh() {					   
	buffer.clear();
	indices.clear();
	for (size_t y = 0; y < size_; y++) {
		for (size_t z = 0; z < size_; z++) {
			for (size_t x = 0; x < size_; x++) {
				const Voxel& voxel = getLocal(x, y, z);

				if (isRender(voxel) == 0) {
					continue;
				}
				float uvsize = atlas_->getVoxelSize();
				//top
				if (global_isUnVisible(x, y + 1, z)) {
					const glm::vec2& uv = atlas_->get(voxel, ::top);

					push_back(indices, buffer.size());
					buffer.push_back(glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y));
					buffer.push_back(glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize));
					buffer.push_back(glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x,			uv.y + uvsize));
					buffer.push_back(glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x,			uv.y));
				}

				if (global_isUnVisible(x, y - 1, z)) {
					const glm::vec2& uv = atlas_->get(voxel, ::bottom);

					push_back(indices, buffer.size());
					buffer.push_back(glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y));
					buffer.push_back(glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y));
					buffer.push_back(glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize));
					buffer.push_back(glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize));
				}

				if (global_isUnVisible(x + 1, y, z)) {
					const glm::vec2& uv = atlas_->get(voxel, ::right);

					push_back(indices, buffer.size());
					buffer.push_back(glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y));
					buffer.push_back(glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize));
					buffer.push_back(glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize));
					buffer.push_back(glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y));
				}

				if (global_isUnVisible(x - 1, y, z)) {
					const glm::vec2& uv = atlas_->get(voxel, ::left);

					push_back(indices, buffer.size());
					buffer.push_back(glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y));
					buffer.push_back(glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y));
					buffer.push_back(glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize));
					buffer.push_back(glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize));
				}

				if (global_isUnVisible(x, y, z + 1)) {
					const glm::vec2& uv = atlas_->get(voxel, ::front);

					push_back(indices, buffer.size());
					buffer.push_back(glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y));
					buffer.push_back(glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y));
					buffer.push_back(glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize));
					buffer.push_back(glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize));
				}
				if (global_isUnVisible(x, y, z - 1)) {
					const glm::vec2& uv = atlas_->get(voxel, ::back);

					push_back(indices, buffer.size());
					buffer.push_back(glm::vec3( x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y));
					buffer.push_back(glm::vec3( x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize));
					buffer.push_back(glm::vec3( x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize));
					buffer.push_back(glm::vec3( x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y));
				}							 					 		
			}
		}
	}
	modified = 0;
}

void Chunk:: draw(const View* view, const Shader& shader) {
	if (modified)
		upMesh();
	shader.use();
	view->use(shader);
	atlas_->use(shader);
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(global_)+glm::vec3(0.5f)  ));
	buffer.getVAO().begin();
	glDrawElements(GlRender::TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
}

///ChunkHandle---------------------------------------------
/// <summary>
/// 
/// </summary>
void ChunkHandle::create(const glm::uvec3& volume) {
	volume_ = volume;
	chunks_.resize(volume_.x * volume_.y * volume_.z);
	size_t index =0;
	for (size_t y = 0; y < volume_.y; y++) {
		for (size_t z = 0; z < volume_.z; z++) {
			for (size_t x = 0; x < volume_.x; x++) {
				chunks_[index].generate(CHUNK_SIZE, glm::ivec3(x, y, z));
				index++;
			}
		}
	}
	for (size_t i = 0; i < chunks_.size(); i++)
		chunks_[i].setCloses(chunks_);
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

void ChunkHandle::setVoxel(const Voxel& voxel , const glm::ivec3& coord) {

	auto local = coord / (int)CHUNK_SIZE;
	if (coord.x < 0) local.x--;
	if (coord.y < 0) local.y--;
	if (coord.z < 0) local.z--;

	Chunk* chunk = get(local);
	if (chunk == 0|| chunk->getGlobal(coord) ==0)return;
	chunk->setVoxel(voxel, coord);
}

Chunk* ChunkHandle::get(const glm::ivec3& coord) {
	if (coord.x < 0|| coord.z < 0|| coord.y < 0 ||coord.x >= volume_.x || coord.y >= volume_.y || coord.z >= volume_.z) return 0;
	return &chunks_[getIndex(coord, volume_)];

}

bool ChunkHandle::setVoxel(const Voxel& voxel, const glm::vec3& start, const glm::vec3& dir, float dist,bool adMode) {
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