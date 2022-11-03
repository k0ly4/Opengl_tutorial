#include "Chunk.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>


//CHunk-------------------------------------
/// <summary>
/// 
/// </summary>

bool Chunk::load(const glm::ivec3& global) {

	local_ = global;
	global_ = global * CHUNK_VOLUME;

	if (FileManager::read(getFilePath(), voxels)) {
		modified = 1;
		closes.setModified();
		return 1;

	}
	return 0;
}

bool Chunk::save()const {
	if (FileManager::write(getFilePath(), voxels)) {
		return 1;
	}
	return 0;
}

void Chunk::generate(const glm::ivec3& global) {
		local_ = global;
		global_ = global * CHUNK_VOLUME;

		voxels.resize(CHUNK_SIZE);
	
		for (size_t y = 0; y < CHUNK_H; y++) {
			for (size_t z = 0; z < CHUNK_D; z++) {
				for (size_t x = 0; x < CHUNK_W; x++)
				{
					glm::ivec3 real_coord = glm::ivec3(x,y,z) + global_;
					int id = (glm::perlin(glm::vec3(real_coord) * 0.0125f) > 0.1f)-1;//real_y <= (height) * 60 + 30;
					if (real_coord.y <= 2)
						id = 1;
					getFromLocalCoord(x, y, z).id = id;
				}
			}
		}

		modified = 1;

}

void Chunk::setCloses(std::vector<Chunk>& chunks) {
	closes.clear();
	for (size_t i = 0; i < chunks.size(); i++) {
		glm::ivec3 dist = chunks[i].local_ - local_;
		if (((abs(dist.x) + abs(dist.y) + abs(dist.z)) != 1))
			continue;
		closes.chunks[getSide(dist)] = &chunks[i];
	}
}



bool Chunk::setVoxel(const Voxel& voxel, const glm::uvec3& coord) {
	Voxel* voxel_ = getFromGlobalCoord(coord);
	if (voxel_ == 0) return 0;
	if (*voxel_ == voxel) return 1;

	*voxel_ = voxel;
	modified = 1;
	closes.setModified(coord - glm::uvec3(global_));
	return 1;
}

inline void push_back(std::vector<unsigned>& indices,size_t begin) {
	indices.push_back(begin);
	indices.push_back(begin + 1);
	indices.push_back(begin + 2);
	indices.push_back(begin);
	indices.push_back(begin + 2);
	indices.push_back(begin + 3);
}

inline void push_back(TypeConvex<VoxelVertex>& convex, const glm::vec3& pos, const glm::vec2& uv,const glm::vec4& light) {
	convex.push_back(VoxelVertex(pos, uv, light));
}

glm::vec4 Chunk::getFastLight(int x, int y, int z) {
	glm::vec4 l_(
		(float)LIGHT(x, y, z, 0) / 15.0f,
		(float)LIGHT(x, y, z, 1) / 15.0f,
		(float)LIGHT(x, y, z, 2) / 15.0f,
		(float)LIGHT(x, y, z, 3) / 15.0f
	);
	return l_;
}

void Chunk::upMesh() {	
	buffer.clear();
	indices.clear();
	for (size_t y = 0; y < CHUNK_H; y++) {
		for (size_t z = 0; z < CHUNK_D; z++) {
			for (size_t x = 0; x < CHUNK_W; x++) {
				const Voxel& voxel = getFromLocalCoord(x, y, z);

				if (isRender(voxel) == 0) {
					continue;
				}
				float uvsize = atlas_->getVoxelSize();
				// AO values
				float l;
				//top
				if (isFree(x, y + 1, z)) {

					l = 1.0f;
					glm::vec4 l_(
						(float)LIGHT(x, y + 1, z, 0) / 15.0f,
						(float)LIGHT(x, y + 1, z, 1) / 15.0f,
						(float)LIGHT(x, y + 1, z, 2) / 15.0f,
						(float)LIGHT(x, y + 1, z, 3) / 15.0f
					);
	
					float lr0 = (LIGHT(x - 1, y + 1, z, 0) + l_.r * 30 + LIGHT(x - 1, y + 1, z - 1, 0) + LIGHT(x, y + 1, z - 1, 0)) / 5.0f / 15.0f;
					float lr1 = (LIGHT(x - 1, y + 1, z, 0) + l_.r * 30 + LIGHT(x - 1, y + 1, z + 1, 0) + LIGHT(x, y + 1, z + 1, 0)) / 5.0f / 15.0f;
					float lr2 = (LIGHT(x + 1, y + 1, z, 0) + l_.r * 30 + LIGHT(x + 1, y + 1, z + 1, 0) + LIGHT(x, y + 1, z + 1, 0)) / 5.0f / 15.0f;
					float lr3 = (LIGHT(x + 1, y + 1, z, 0) + l_.r * 30 + LIGHT(x + 1, y + 1, z - 1, 0) + LIGHT(x, y + 1, z - 1, 0)) / 5.0f / 15.0f;

					float lg0 = (LIGHT(x - 1, y + 1, z, 1) + l_.g * 30 + LIGHT(x - 1, y + 1, z - 1, 1) + LIGHT(x, y + 1, z - 1, 1)) / 5.0f / 15.0f;
					float lg1 = (LIGHT(x - 1, y + 1, z, 1) + l_.g * 30 + LIGHT(x - 1, y + 1, z + 1, 1) + LIGHT(x, y + 1, z + 1, 1)) / 5.0f / 15.0f;
					float lg2 = (LIGHT(x + 1, y + 1, z, 1) + l_.g * 30 + LIGHT(x + 1, y + 1, z + 1, 1) + LIGHT(x, y + 1, z + 1, 1)) / 5.0f / 15.0f;
					float lg3 = (LIGHT(x + 1, y + 1, z, 1) + l_.g * 30 + LIGHT(x + 1, y + 1, z - 1, 1) + LIGHT(x, y + 1, z - 1, 1)) / 5.0f / 15.0f;

					float lb0 = (LIGHT(x - 1, y + 1, z, 2) + l_.b * 30 + LIGHT(x - 1, y + 1, z - 1, 2) + LIGHT(x, y + 1, z - 1, 2)) / 5.0f / 15.0f;
					float lb1 = (LIGHT(x - 1, y + 1, z, 2) + l_.b * 30 + LIGHT(x - 1, y + 1, z + 1, 2) + LIGHT(x, y + 1, z + 1, 2)) / 5.0f / 15.0f;
					float lb2 = (LIGHT(x + 1, y + 1, z, 2) + l_.b * 30 + LIGHT(x + 1, y + 1, z + 1, 2) + LIGHT(x, y + 1, z + 1, 2)) / 5.0f / 15.0f;
					float lb3 = (LIGHT(x + 1, y + 1, z, 2) + l_.b * 30 + LIGHT(x + 1, y + 1, z - 1, 2) + LIGHT(x, y + 1, z - 1, 2)) / 5.0f / 15.0f;

					float ls0 = (LIGHT(x - 1, y + 1, z, 3) + l_.a * 30 + LIGHT(x - 1, y + 1, z - 1, 3) + LIGHT(x, y + 1, z - 1, 3)) / 5.0f / 15.0f;
					float ls1 = (LIGHT(x - 1, y + 1, z, 3) + l_.a * 30 + LIGHT(x - 1, y + 1, z + 1, 3) + LIGHT(x, y + 1, z + 1, 3)) / 5.0f / 15.0f;
					float ls2 = (LIGHT(x + 1, y + 1, z, 3) + l_.a * 30 + LIGHT(x + 1, y + 1, z + 1, 3) + LIGHT(x, y + 1, z + 1, 3)) / 5.0f / 15.0f;
					float ls3 = (LIGHT(x + 1, y + 1, z, 3) + l_.a * 30 + LIGHT(x + 1, y + 1, z - 1, 3) + LIGHT(x, y + 1, z - 1, 3)) / 5.0f / 15.0f;

					const glm::vec2& uv = atlas_->get(voxel, ::top);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr0, lg0, lb0, ls0));
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(lr1, lg1, lb1, ls1));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x,			uv.y + uvsize), glm::vec4(lr2, lg2, lb2, ls2));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x,			uv.y),			glm::vec4(lr3, lg3, lb3, ls3));
				}

				if (isFree(x, y - 1, z)) {
					l = 0.75f;

					float lr = LIGHT(x, y - 1, z, 0) / 15.0f;
					float lg = LIGHT(x, y - 1, z, 1) / 15.0f;
					float lb = LIGHT(x, y - 1, z, 2) / 15.0f;
					float ls = LIGHT(x, y - 1, z, 3) / 15.0f;

					float lr0 = (LIGHT(x - 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x - 1, y - 1, z, 0) + LIGHT(x, y - 1, z - 1, 0)) / 5.0f / 15.0f;
					float lr1 = (LIGHT(x + 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x + 1, y - 1, z, 0) + LIGHT(x, y - 1, z + 1, 0)) / 5.0f / 15.0f;
					float lr2 = (LIGHT(x - 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x - 1, y - 1, z, 0) + LIGHT(x, y - 1, z + 1, 0)) / 5.0f / 15.0f;
					float lr3 = (LIGHT(x + 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x + 1, y - 1, z, 0) + LIGHT(x, y - 1, z - 1, 0)) / 5.0f / 15.0f;

					float lg0 = (LIGHT(x - 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x - 1, y - 1, z, 1) + LIGHT(x, y - 1, z - 1, 1)) / 5.0f / 15.0f;
					float lg1 = (LIGHT(x + 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x + 1, y - 1, z, 1) + LIGHT(x, y - 1, z + 1, 1)) / 5.0f / 15.0f;
					float lg2 = (LIGHT(x - 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x - 1, y - 1, z, 1) + LIGHT(x, y - 1, z + 1, 1)) / 5.0f / 15.0f;
					float lg3 = (LIGHT(x + 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x + 1, y - 1, z, 1) + LIGHT(x, y - 1, z - 1, 1)) / 5.0f / 15.0f;

					float lb0 = (LIGHT(x - 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x - 1, y - 1, z, 2) + LIGHT(x, y - 1, z - 1, 2)) / 5.0f / 15.0f;
					float lb1 = (LIGHT(x + 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x + 1, y - 1, z, 2) + LIGHT(x, y - 1, z + 1, 2)) / 5.0f / 15.0f;
					float lb2 = (LIGHT(x - 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x - 1, y - 1, z, 2) + LIGHT(x, y - 1, z + 1, 2)) / 5.0f / 15.0f;
					float lb3 = (LIGHT(x + 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x + 1, y - 1, z, 2) + LIGHT(x, y - 1, z - 1, 2)) / 5.0f / 15.0f;

					float ls0 = (LIGHT(x - 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x - 1, y - 1, z, 3) + LIGHT(x, y - 1, z - 1, 3)) / 5.0f / 15.0f;
					float ls1 = (LIGHT(x + 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x + 1, y - 1, z, 3) + LIGHT(x, y - 1, z + 1, 3)) / 5.0f / 15.0f;
					float ls2 = (LIGHT(x - 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x - 1, y - 1, z, 3) + LIGHT(x, y - 1, z + 1, 3)) / 5.0f / 15.0f;
					float ls3 = (LIGHT(x + 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x + 1, y - 1, z, 3) + LIGHT(x, y - 1, z - 1, 3)) / 5.0f / 15.0f;

					const glm::vec2& uv = atlas_->get(voxel, ::bottom);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr0, lg0, lb0, ls0));
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr3, lg3, lb3, ls3));
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),  glm::vec4(lr1, lg1, lb1, ls1));
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
				}

				if (isFree(x + 1, y, z)) {
					l = 0.95f;
					float lr = LIGHT(x + 1, y, z, 0) / 15.0f;
					float lg = LIGHT(x + 1, y, z, 1) / 15.0f;
					float lb = LIGHT(x + 1, y, z, 2) / 15.0f;
					float ls = LIGHT(x + 1, y, z, 3) / 15.0f;

					float lr0 = (LIGHT(x + 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x + 1, y, z - 1, 0) + LIGHT(x + 1, y - 1, z, 0)) / 5.0f / 15.0f;
					float lr1 = (LIGHT(x + 1, y + 1, z - 1, 0) + lr * 30 + LIGHT(x + 1, y, z - 1, 0) + LIGHT(x + 1, y + 1, z, 0)) / 5.0f / 15.0f;
					float lr2 = (LIGHT(x + 1, y + 1, z + 1, 0) + lr * 30 + LIGHT(x + 1, y, z + 1, 0) + LIGHT(x + 1, y + 1, z, 0)) / 5.0f / 15.0f;
					float lr3 = (LIGHT(x + 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x + 1, y, z + 1, 0) + LIGHT(x + 1, y - 1, z, 0)) / 5.0f / 15.0f;

					float lg0 = (LIGHT(x + 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x + 1, y, z - 1, 1) + LIGHT(x + 1, y - 1, z, 1)) / 5.0f / 15.0f;
					float lg1 = (LIGHT(x + 1, y + 1, z - 1, 1) + lg * 30 + LIGHT(x + 1, y, z - 1, 1) + LIGHT(x + 1, y + 1, z, 1)) / 5.0f / 15.0f;
					float lg2 = (LIGHT(x + 1, y + 1, z + 1, 1) + lg * 30 + LIGHT(x + 1, y, z + 1, 1) + LIGHT(x + 1, y + 1, z, 1)) / 5.0f / 15.0f;
					float lg3 = (LIGHT(x + 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x + 1, y, z + 1, 1) + LIGHT(x + 1, y - 1, z, 1)) / 5.0f / 15.0f;

					float lb0 = (LIGHT(x + 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x + 1, y, z - 1, 2) + LIGHT(x + 1, y - 1, z, 2)) / 5.0f / 15.0f;
					float lb1 = (LIGHT(x + 1, y + 1, z - 1, 2) + lb * 30 + LIGHT(x + 1, y, z - 1, 2) + LIGHT(x + 1, y + 1, z, 2)) / 5.0f / 15.0f;
					float lb2 = (LIGHT(x + 1, y + 1, z + 1, 2) + lb * 30 + LIGHT(x + 1, y, z + 1, 2) + LIGHT(x + 1, y + 1, z, 2)) / 5.0f / 15.0f;
					float lb3 = (LIGHT(x + 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x + 1, y, z + 1, 2) + LIGHT(x + 1, y - 1, z, 2)) / 5.0f / 15.0f;

					float ls0 = (LIGHT(x + 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x + 1, y, z - 1, 3) + LIGHT(x + 1, y - 1, z, 3)) / 5.0f / 15.0f;
					float ls1 = (LIGHT(x + 1, y + 1, z - 1, 3) + ls * 30 + LIGHT(x + 1, y, z - 1, 3) + LIGHT(x + 1, y + 1, z, 3)) / 5.0f / 15.0f;
					float ls2 = (LIGHT(x + 1, y + 1, z + 1, 3) + ls * 30 + LIGHT(x + 1, y, z + 1, 3) + LIGHT(x + 1, y + 1, z, 3)) / 5.0f / 15.0f;
					float ls3 = (LIGHT(x + 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x + 1, y, z + 1, 3) + LIGHT(x + 1, y - 1, z, 3)) / 5.0f / 15.0f;

					const glm::vec2& uv = atlas_->get(voxel, ::right);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr0, lg0, lb0, ls0));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(lr1, lg1, lb1, ls1));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr3, lg3, lb3, ls3));
				}

				if (isFree(x - 1, y, z)) {
					l = 0.85f;

					float lr = LIGHT(x - 1, y, z, 0) / 15.0f;
					float lg = LIGHT(x - 1, y, z, 1) / 15.0f;
					float lb = LIGHT(x - 1, y, z, 2) / 15.0f;
					float ls = LIGHT(x - 1, y, z, 3) / 15.0f;

					float lr0 = (LIGHT(x - 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x - 1, y, z - 1, 0) + LIGHT(x - 1, y - 1, z, 0)) / 5.0f / 15.0f;
					float lr1 = (LIGHT(x - 1, y + 1, z + 1, 0) + lr * 30 + LIGHT(x - 1, y, z + 1, 0) + LIGHT(x - 1, y + 1, z, 0)) / 5.0f / 15.0f;
					float lr2 = (LIGHT(x - 1, y + 1, z - 1, 0) + lr * 30 + LIGHT(x - 1, y, z - 1, 0) + LIGHT(x - 1, y + 1, z, 0)) / 5.0f / 15.0f;
					float lr3 = (LIGHT(x - 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x - 1, y, z + 1, 0) + LIGHT(x - 1, y - 1, z, 0)) / 5.0f / 15.0f;

					float lg0 = (LIGHT(x - 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x - 1, y, z - 1, 1) + LIGHT(x - 1, y - 1, z, 1)) / 5.0f / 15.0f;
					float lg1 = (LIGHT(x - 1, y + 1, z + 1, 1) + lg * 30 + LIGHT(x - 1, y, z + 1, 1) + LIGHT(x - 1, y + 1, z, 1)) / 5.0f / 15.0f;
					float lg2 = (LIGHT(x - 1, y + 1, z - 1, 1) + lg * 30 + LIGHT(x - 1, y, z - 1, 1) + LIGHT(x - 1, y + 1, z, 1)) / 5.0f / 15.0f;
					float lg3 = (LIGHT(x - 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x - 1, y, z + 1, 1) + LIGHT(x - 1, y - 1, z, 1)) / 5.0f / 15.0f;

					float lb0 = (LIGHT(x - 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x - 1, y, z - 1, 2) + LIGHT(x - 1, y - 1, z, 2)) / 5.0f / 15.0f;
					float lb1 = (LIGHT(x - 1, y + 1, z + 1, 2) + lb * 30 + LIGHT(x - 1, y, z + 1, 2) + LIGHT(x - 1, y + 1, z, 2)) / 5.0f / 15.0f;
					float lb2 = (LIGHT(x - 1, y + 1, z - 1, 2) + lb * 30 + LIGHT(x - 1, y, z - 1, 2) + LIGHT(x - 1, y + 1, z, 2)) / 5.0f / 15.0f;
					float lb3 = (LIGHT(x - 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x - 1, y, z + 1, 2) + LIGHT(x - 1, y - 1, z, 2)) / 5.0f / 15.0f;

					float ls0 = (LIGHT(x - 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x - 1, y, z - 1, 3) + LIGHT(x - 1, y - 1, z, 3)) / 5.0f / 15.0f;
					float ls1 = (LIGHT(x - 1, y + 1, z + 1, 3) + ls * 30 + LIGHT(x - 1, y, z + 1, 3) + LIGHT(x - 1, y + 1, z, 3)) / 5.0f / 15.0f;
					float ls2 = (LIGHT(x - 1, y + 1, z - 1, 3) + ls * 30 + LIGHT(x - 1, y, z - 1, 3) + LIGHT(x - 1, y + 1, z, 3)) / 5.0f / 15.0f;
					float ls3 = (LIGHT(x - 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x - 1, y, z + 1, 3) + LIGHT(x - 1, y - 1, z, 3)) / 5.0f / 15.0f;

			
					const glm::vec2& uv = atlas_->get(voxel, ::left);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr0, lg0, lb0, ls0));
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr3, lg3, lb3, ls3));
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(lr1, lg1, lb1, ls1));
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
				}

				if (isFree(x, y, z + 1)) {
					l = 0.9f;

					float lr = LIGHT(x, y, z + 1, 0) / 15.0f;
					float lg = LIGHT(x, y, z + 1, 1) / 15.0f;
					float lb = LIGHT(x, y, z + 1, 2) / 15.0f;
					float ls = LIGHT(x, y, z + 1, 3) / 15.0f;

					float lr0 = l * (LIGHT(x - 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x, y - 1, z + 1, 0) + LIGHT(x - 1, y, z + 1, 0)) / 5.0f / 15.0f;
					float lr1 = l * (LIGHT(x + 1, y + 1, z + 1, 0) + lr * 30 + LIGHT(x, y + 1, z + 1, 0) + LIGHT(x + 1, y, z + 1, 0)) / 5.0f / 15.0f;
					float lr2 = l * (LIGHT(x - 1, y + 1, z + 1, 0) + lr * 30 + LIGHT(x, y + 1, z + 1, 0) + LIGHT(x - 1, y, z + 1, 0)) / 5.0f / 15.0f;
					float lr3 = l * (LIGHT(x + 1, y - 1, z + 1, 0) + lr * 30 + LIGHT(x, y - 1, z + 1, 0) + LIGHT(x + 1, y, z + 1, 0)) / 5.0f / 15.0f;

					float lg0 = l * (LIGHT(x - 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x, y - 1, z + 1, 1) + LIGHT(x - 1, y, z + 1, 1)) / 5.0f / 15.0f;
					float lg1 = l * (LIGHT(x + 1, y + 1, z + 1, 1) + lg * 30 + LIGHT(x, y + 1, z + 1, 1) + LIGHT(x + 1, y, z + 1, 1)) / 5.0f / 15.0f;
					float lg2 = l * (LIGHT(x - 1, y + 1, z + 1, 1) + lg * 30 + LIGHT(x, y + 1, z + 1, 1) + LIGHT(x - 1, y, z + 1, 1)) / 5.0f / 15.0f;
					float lg3 = l * (LIGHT(x + 1, y - 1, z + 1, 1) + lg * 30 + LIGHT(x, y - 1, z + 1, 1) + LIGHT(x + 1, y, z + 1, 1)) / 5.0f / 15.0f;

					float lb0 = l * (LIGHT(x - 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x, y - 1, z + 1, 2) + LIGHT(x - 1, y, z + 1, 2)) / 5.0f / 15.0f;
					float lb1 = l * (LIGHT(x + 1, y + 1, z + 1, 2) + lb * 30 + LIGHT(x, y + 1, z + 1, 2) + LIGHT(x + 1, y, z + 1, 2)) / 5.0f / 15.0f;
					float lb2 = l * (LIGHT(x - 1, y + 1, z + 1, 2) + lb * 30 + LIGHT(x, y + 1, z + 1, 2) + LIGHT(x - 1, y, z + 1, 2)) / 5.0f / 15.0f;
					float lb3 = l * (LIGHT(x + 1, y - 1, z + 1, 2) + lb * 30 + LIGHT(x, y - 1, z + 1, 2) + LIGHT(x + 1, y, z + 1, 2)) / 5.0f / 15.0f;

					float ls0 = l * (LIGHT(x - 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x, y - 1, z + 1, 3) + LIGHT(x - 1, y, z + 1, 3)) / 5.0f / 15.0f;
					float ls1 = l * (LIGHT(x + 1, y + 1, z + 1, 3) + ls * 30 + LIGHT(x, y + 1, z + 1, 3) + LIGHT(x + 1, y, z + 1, 3)) / 5.0f / 15.0f;
					float ls2 = l * (LIGHT(x - 1, y + 1, z + 1, 3) + ls * 30 + LIGHT(x, y + 1, z + 1, 3) + LIGHT(x - 1, y, z + 1, 3)) / 5.0f / 15.0f;
					float ls3 = l * (LIGHT(x + 1, y - 1, z + 1, 3) + ls * 30 + LIGHT(x, y - 1, z + 1, 3) + LIGHT(x + 1, y, z + 1, 3)) / 5.0f / 15.0f;

					const glm::vec2& uv = atlas_->get(voxel, ::front);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr0, lg0, lb0, ls0));
					push_back(buffer,glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr3, lg3, lb3, ls3));
					push_back(buffer,glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(lr1, lg1, lb1, ls1));
					push_back(buffer,glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
				}
				if (isFree(x, y, z - 1)) {
					l = 0.8f;
					float lr = LIGHT(x, y, z - 1, 0) / 15.0f;
					float lg = LIGHT(x, y, z - 1, 1) / 15.0f;
					float lb = LIGHT(x, y, z - 1, 2) / 15.0f;
					float ls = LIGHT(x, y, z - 1, 3) / 15.0f;

					float lr0 = l * (LIGHT(x - 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x, y - 1, z - 1, 0) + LIGHT(x - 1, y, z - 1, 0)) / 5.0f / 15.0f;
					float lr1 = l * (LIGHT(x - 1, y + 1, z - 1, 0) + lr * 30 + LIGHT(x, y + 1, z - 1, 0) + LIGHT(x - 1, y, z - 1, 0)) / 5.0f / 15.0f;
					float lr2 = l * (LIGHT(x + 1, y + 1, z - 1, 0) + lr * 30 + LIGHT(x, y + 1, z - 1, 0) + LIGHT(x + 1, y, z - 1, 0)) / 5.0f / 15.0f;
					float lr3 = l * (LIGHT(x + 1, y - 1, z - 1, 0) + lr * 30 + LIGHT(x, y - 1, z - 1, 0) + LIGHT(x + 1, y, z - 1, 0)) / 5.0f / 15.0f;

					float lg0 = l * (LIGHT(x - 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x, y - 1, z - 1, 1) + LIGHT(x - 1, y, z - 1, 1)) / 5.0f / 15.0f;
					float lg1 = l * (LIGHT(x - 1, y + 1, z - 1, 1) + lg * 30 + LIGHT(x, y + 1, z - 1, 1) + LIGHT(x - 1, y, z - 1, 1)) / 5.0f / 15.0f;
					float lg2 = l * (LIGHT(x + 1, y + 1, z - 1, 1) + lg * 30 + LIGHT(x, y + 1, z - 1, 1) + LIGHT(x + 1, y, z - 1, 1)) / 5.0f / 15.0f;
					float lg3 = l * (LIGHT(x + 1, y - 1, z - 1, 1) + lg * 30 + LIGHT(x, y - 1, z - 1, 1) + LIGHT(x + 1, y, z - 1, 1)) / 5.0f / 15.0f;

					float lb0 = l * (LIGHT(x - 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x, y - 1, z - 1, 2) + LIGHT(x - 1, y, z - 1, 2)) / 5.0f / 15.0f;
					float lb1 = l * (LIGHT(x - 1, y + 1, z - 1, 2) + lb * 30 + LIGHT(x, y + 1, z - 1, 2) + LIGHT(x - 1, y, z - 1, 2)) / 5.0f / 15.0f;
					float lb2 = l * (LIGHT(x + 1, y + 1, z - 1, 2) + lb * 30 + LIGHT(x, y + 1, z - 1, 2) + LIGHT(x + 1, y, z - 1, 2)) / 5.0f / 15.0f;
					float lb3 = l * (LIGHT(x + 1, y - 1, z - 1, 2) + lb * 30 + LIGHT(x, y - 1, z - 1, 2) + LIGHT(x + 1, y, z - 1, 2)) / 5.0f / 15.0f;

					float ls0 = l * (LIGHT(x - 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x, y - 1, z - 1, 3) + LIGHT(x - 1, y, z - 1, 3)) / 5.0f / 15.0f;
					float ls1 = l * (LIGHT(x - 1, y + 1, z - 1, 3) + ls * 30 + LIGHT(x, y + 1, z - 1, 3) + LIGHT(x - 1, y, z - 1, 3)) / 5.0f / 15.0f;
					float ls2 = l * (LIGHT(x + 1, y + 1, z - 1, 3) + ls * 30 + LIGHT(x, y + 1, z - 1, 3) + LIGHT(x + 1, y, z - 1, 3)) / 5.0f / 15.0f;
					float ls3 = l * (LIGHT(x + 1, y - 1, z - 1, 3) + ls * 30 + LIGHT(x, y - 1, z - 1, 3) + LIGHT(x + 1, y, z - 1, 3)) / 5.0f / 15.0f;

					const glm::vec2& uv = atlas_->get(voxel, ::back);

					push_back(indices, buffer.size());
					push_back(buffer,glm::vec3( x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr0, lg0, lb0, ls0));
					push_back(buffer,glm::vec3( x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), glm::vec4(lr1, lg1, lb1, ls1));
					push_back(buffer,glm::vec3( x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
					push_back(buffer,glm::vec3( x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr3, lg3, lb3, ls3));
				}							 					 		
			}
		}
	}
	modified = 0;
}

void Chunk::fastUpMesh() {
	buffer.clear();
	indices.clear();
	for (size_t y = 0; y < CHUNK_H; y++) {
		for (size_t z = 0; z < CHUNK_D; z++) {
			for (size_t x = 0; x < CHUNK_W; x++) {
				const Voxel& voxel = getFromLocalCoord(x, y, z);

				if (isRender(voxel) == 0) {
					continue;
				}
				float uvsize = atlas_->getVoxelSize();
				//top
				if (isFree(x, y + 1, z)) {
					glm::vec4 light = getFastLight(x, y + 1, z);
				
					const glm::vec2& uv = atlas_->get(voxel, ::top);	

					push_back(indices, buffer.size());
					push_back(buffer, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back(buffer, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back(buffer, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
					push_back(buffer, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					light);
				}

				if (isFree(x, y - 1, z)) {
					glm::vec4 light = getFastLight(x, y - 1, z);
				
					const glm::vec2& uv = atlas_->get(voxel, ::bottom);

					push_back(indices, buffer.size());
					push_back(buffer, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					light);
					push_back(buffer, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back(buffer, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back(buffer, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
				}

				if (isFree(x + 1, y, z)) {
					glm::vec4 light = getFastLight(x + 1, y, z);
				
					const glm::vec2& uv = atlas_->get(voxel, ::right);

					push_back(indices, buffer.size());
					push_back(buffer, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back(buffer, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back(buffer, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
					push_back(buffer, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					light);
				}

				if (isFree(x - 1, y, z)) {
					glm::vec4 light = getFastLight(x - 1, y, z);

					const glm::vec2& uv = atlas_->get(voxel, ::left);

					push_back(indices, buffer.size());
					push_back(buffer, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					light);
					push_back(buffer, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back(buffer, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back(buffer, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
				}

				if (isFree(x, y, z + 1)) {
					glm::vec4 light = getFastLight(x, y, z + 1);
					
					const glm::vec2& uv = atlas_->get(voxel, ::front);

					push_back(indices, buffer.size());
					push_back(buffer, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					light);
					push_back(buffer, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back(buffer, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back(buffer, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
				}
				if (isFree(x, y, z - 1)) {
					glm::vec4 light = getFastLight(x, y, z - 1);
					
					const glm::vec2& uv = atlas_->get(voxel, ::back);

					push_back(indices, buffer.size());
					push_back(buffer, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back(buffer, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back(buffer, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
					push_back(buffer, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					light);
				}
			}
		}
	}
	modified = 0;
}

void Chunk:: draw(const View* view, const Shader& shader) {
	if (modified) upMesh();
	shader.use();
	view->useVP(shader);
	atlas_->use(shader);
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(global_)+glm::vec3(0.5f)  ));
	buffer.getVAO().begin();
	glDrawElements(GlRender::TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
}

