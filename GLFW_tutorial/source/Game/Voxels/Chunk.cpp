#include "Chunk.h"

//CHunk-------------------------------------
/// <summary>
/// 
/// </summary>

bool gChunk::setVoxel(const Voxel& voxel, const glm::uvec3& coord) {
	Voxel* voxel_ = getFromGlobalCoord(coord);
	if (voxel_ == 0) return 0;
	if (*voxel_ == voxel) return 1;

	*voxel_ = voxel;
	modified = 1;
	closes.setModified(coord.x - global_.x, coord.z- global_.z);
	return 1;
}



inline void push_back(iGeometry<VoxelVertex>& convex, const glm::vec3& pos, const glm::vec2& uv,const glm::vec4& light) {
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

void Chunk::buildMesh() {	
	mesh.vertices.clear();
	mesh.indices.clear();
	for (size_t y = 0; y < CHUNK_H; y++) {
		for (size_t z = 0; z < CHUNK_D; z++) {
			for (size_t x = 0; x < CHUNK_W; x++) {
				const Voxel& voxel = getFromLocalCoord(x, y, z);

				if (VoxelPack::isRender(voxel) == 0) {
					continue;
				}
				float uvsize = VoxelPack::get()->getVoxelSize();
				
				float l;
				byte drawGroup = VoxelPack::get(voxel).drawGroup;
				//top
				if (isFree(x, y + 1, z, drawGroup)) {

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

					const glm::vec2& uv = VoxelPack::get(voxel, Side::top);

					mesh.pushIndices(mesh.vertices.size());
					push_back(mesh,glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr0, lg0, lb0, ls0));
					push_back(mesh,glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(lr1, lg1, lb1, ls1));
					push_back(mesh,glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x,			uv.y + uvsize), glm::vec4(lr2, lg2, lb2, ls2));
					push_back(mesh,glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x,			uv.y),			glm::vec4(lr3, lg3, lb3, ls3));
				}

				if (isFree(x, y - 1, z, drawGroup)) {
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

					const glm::vec2& uv = VoxelPack::get(voxel, Side::bottom);

					mesh.pushIndices(mesh.vertices.size());
					push_back(mesh,glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr0, lg0, lb0, ls0));
					push_back(mesh,glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr3, lg3, lb3, ls3));
					push_back(mesh,glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),  glm::vec4(lr1, lg1, lb1, ls1));
					push_back(mesh,glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
				}

				if (isFree(x + 1, y, z, drawGroup)) {
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

					const glm::vec2& uv = VoxelPack::get(voxel, Side::right);

					mesh.pushIndices(mesh.vertices.size());
					push_back(mesh,glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr0, lg0, lb0, ls0));
					push_back(mesh,glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(lr1, lg1, lb1, ls1));
					push_back(mesh,glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
					push_back(mesh,glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr3, lg3, lb3, ls3));
				}

				if (isFree(x - 1, y, z, drawGroup)) {
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

			
					const glm::vec2& uv = VoxelPack::get(voxel, Side::left);

					mesh.pushIndices(mesh.vertices.size());
					push_back(mesh,glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr0, lg0, lb0, ls0));
					push_back(mesh,glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr3, lg3, lb3, ls3));
					push_back(mesh,glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(lr1, lg1, lb1, ls1));
					push_back(mesh,glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
				}

				if (isFree(x, y, z + 1, drawGroup)) {
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

					const glm::vec2& uv = VoxelPack::get(voxel, Side::front);

					mesh.pushIndices(mesh.vertices.size());
					push_back(mesh,glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr0, lg0, lb0, ls0));
					push_back(mesh,glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr3, lg3, lb3, ls3));
					push_back(mesh,glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	glm::vec4(lr1, lg1, lb1, ls1));
					push_back(mesh,glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
				}
				if (isFree(x, y, z - 1, drawGroup)) {
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

					const glm::vec2& uv = VoxelPack::get(voxel, Side::back);

					mesh.pushIndices(mesh.vertices.size());
					push_back(mesh,glm::vec3( x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			glm::vec4(lr0, lg0, lb0, ls0));
					push_back(mesh,glm::vec3( x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), glm::vec4(lr1, lg1, lb1, ls1));
					push_back(mesh,glm::vec3( x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			glm::vec4(lr2, lg2, lb2, ls2));
					push_back(mesh,glm::vec3( x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					glm::vec4(lr3, lg3, lb3, ls3));
				}							 					 		
			}
		}
	}
	needUpBuffer = 1;
	modified = 0;
}

void Chunk::fastUpMesh() {
	modified = 0;
	mesh.vertices.clear();
	mesh.indices.clear();
	for (size_t y = 0; y < CHUNK_H; y++) {
		for (size_t z = 0; z < CHUNK_D; z++) {
			for (size_t x = 0; x < CHUNK_W; x++) {
				const Voxel& voxel = getFromLocalCoord(x, y, z);

				if (VoxelPack::isRender(voxel) == 0) {
					continue;
				}
				float uvsize = VoxelPack::get()->getVoxelSize();
				byte drawGroup = VoxelPack::get()->get(voxel).drawGroup;
				//top
				if (isFree(x, y + 1, z, drawGroup)) {
					glm::vec4 light = getFastLight(x, y + 1, z);
				
					const glm::vec2& uv = VoxelPack::get(voxel, Side::top);

					mesh.pushIndices(mesh.vertices.size());
					push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
					push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					light);
				}

				if (isFree(x, y - 1, z, drawGroup)) {
					glm::vec4 light = getFastLight(x, y - 1, z);
				
					const glm::vec2& uv = VoxelPack::get(voxel, Side::bottom);

					mesh.pushIndices(mesh.vertices.size());
					push_back( mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					light);
					push_back( mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back( mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back( mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
				}

				if (isFree(x + 1, y, z, drawGroup)) {
					glm::vec4 light = getFastLight(x + 1, y, z);
				
					const glm::vec2& uv = VoxelPack::get(voxel, Side::right);

					mesh.pushIndices(mesh.vertices.size());
					push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
					push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					light);
				}

				if (isFree(x - 1, y, z, drawGroup)) {
					glm::vec4 light = getFastLight(x - 1, y, z);

					const glm::vec2& uv = VoxelPack::get(voxel, Side::left);

					mesh.pushIndices(mesh.vertices.size());
					push_back( mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					light);
					push_back( mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back( mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back( mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
				}

				if (isFree(x, y, z + 1, drawGroup)) {
					glm::vec4 light = getFastLight(x, y, z + 1);
					
					const glm::vec2& uv = VoxelPack::get(voxel, Side::front);

					mesh.pushIndices(mesh.vertices.size());
					push_back( mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y),					light);
					push_back( mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back( mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back( mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
				}
				if (isFree(x, y, z - 1, drawGroup)) {
					glm::vec4 light = getFastLight(x, y, z - 1);
					
					const glm::vec2& uv = VoxelPack::get(voxel, Side::back);

					mesh.pushIndices(mesh.vertices.size());
					push_back( mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y),			light);
					push_back( mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), light);
					push_back( mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			light);
					push_back( mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),					light);
				}
			}
		}
	}
	needUpBuffer = 1;
	modified = 0;
}

void Chunk::draw(const Shader& shader) {
	if (needUpBuffer)
	{
		needUpBuffer = 0;
		mesh.saveInBuffer();
	}
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(global_) + glm::vec3(0.5f)));
	mesh.draw();
}

void Chunk::draw(const View* view, const Shader& shader) {
	if (needUpBuffer)
	{
		needUpBuffer = 0;
		mesh.saveInBuffer();
	}
	
	shader.use();
	view->useVP(shader);
	VoxelPack::get()->use(shader);
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(global_) + glm::vec3(0.5f)  ));
	mesh.draw();
}
//////ReaderChunk----------------------------------
