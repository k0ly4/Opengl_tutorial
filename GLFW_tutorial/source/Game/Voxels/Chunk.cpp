#include "Chunk.h"
#include "Game/Voxels/ChunkMesh.h"
//CHunk-------------------------------------
/// <summary>
/// 
/// </summary>

void gChunk::setVoxelLocal(Voxel voxel, const glm::uvec3& coord) {
	//if (voxel.e.id == vox::water)LOG("setVoxelLocal", VoxPack::isActive(voxel));
	size_t index = voxs.ind(coord);
	Voxel dest = voxs[index];
	//��������� �� �� �������
	if (dest == voxel)return;
	voxs[index] = voxel;
	//��������� ��� ����������� �����������
	if (VoxPack::isAlpha(voxel) && ((VoxPack::isAlpha(dest) || VoxPack::isRender(dest) == 0))) {
		flag.modAlphMesh();
		closes.modifyAlpha(coord.x, coord.z);
	}
	else {
		flag.modMesh();
		closes.modify(coord.x, coord.z);
	}	
	//���������� �����
	if (VoxPack::isActive(voxel)) toPhysBuffer(index);
	glm::ivec3 off[6] = {
				glm::ivec3(-1, 0, 0),
				glm::ivec3(1, 0, 0),
				glm::ivec3(0, -1, 0),
				glm::ivec3(0, 1, 0) ,
				glm::ivec3(0, 0, -1) ,
				glm::ivec3(0, 0, 1)
	};
	for (size_t i = 0; i < 6; i++) {
		glm::ivec3 pos(glm::ivec3(coord) + off[i]);
		if ((size_t)pos.y >= CHUNK_H) continue;
		if (voxs.is(pos)) { if (VoxPack::isActive(voxs(pos))) toPhysBuffer(pos); }
		else {
			gChunk* cl = closes.chunk(pos.x, pos.z);
			pos = { clip(pos.x,CHUNK_W),pos.y,clip(pos.z,CHUNK_D) };
			if (cl && VoxPack::isActive(cl->voxs(pos))) cl->toPhysBuffer(pos);
		}

	}
}

void ChunkGraphic::buildMesh() {
	if ((flag.status < StateChunk::s_mesh)) return;
	cBlock = 1;
	while (dBlock)std::this_thread::yield();
	sChunkMesh::buildOpaqueMesh(*this);
	cBlock = 0;
}

void  ChunkGraphic::buildSortMesh(glm::ivec3 pos) {
	
	pos = pos- glm::ivec3(bg_vox);
	if(flag.status < StateChunk::s_alph_mesh) return;
	if ((posView_ == pos)  &&(flag.status > StateChunk::s_alph_mesh))return;
	//sync
	cBlock = 1;
	while (dBlock)std::this_thread::yield();

	posView_ = pos;
	sChunkMesh::buildSortMesh(*this);
	cBlock = 0;
}

void ChunkGraphic::drawOpaqueMesh(const Shader& shader) {
	if ((flag.status > StateChunk::s_mesh) && mesh_.needUpBuffer)
	{
		dBlock = 1;
		while (cBlock)std::this_thread::yield();
		
		mesh_.needUpBuffer = 0;
		//LOG("UpBuffer\n");
		mesh_.saveInBuffer();
		dBlock = 0;
		
	}
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(bg_vox)));
	mesh_.draw();
}

void ChunkGraphic::drawSortMesh(const Shader& shader) {
	if ((flag.status == StateChunk::s_ready) && mesh_sort.needUpBuffer)
	{
		dBlock = 1;
		while (cBlock)std::this_thread::yield();
		mesh_sort.needUpBuffer = 0;
		mesh_sort.saveInBuffer();
		dBlock = 0;
	}
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(bg_vox)));
	mesh_sort.draw();
}
