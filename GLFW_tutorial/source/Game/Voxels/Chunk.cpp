#include "Chunk.h"

//CHunk-------------------------------------
/// <summary>
/// 
/// </summary>

bool gChunk::setVoxel(const Voxel& voxel, const glm::uvec3& coord) {
	Voxel* voxel_ = getGlobal(coord);
	if (voxel_ == 0) return 0;
	if (*voxel_ == voxel) return 1;

	*voxel_ = voxel;
	modified = 1;
	closes.setModified(coord.x - global_.x, coord.z- global_.z);
	return 1;
}

inline glm::vec4 Chunk::getFastLight(int x, int y, int z) {
	return 
	glm::vec4(
		(float)LIGHT(x, y, z, 0),
		(float)LIGHT(x, y, z, 1),
		(float)LIGHT(x, y, z, 2),
		(float)LIGHT(x, y, z, 3)
	);
}

const float cLight = 1.f / 70.f;
//Top-------------------------
void Chunk::getLightTop(int x, int yT, int z, LightFace& face) {
	Vec4M l(getFastLight(x, yT, z));
	for (size_t i = 0; i < 4; i++) {
		face.l0m[i] = (LIGHT(x - 1, yT, z, i) + l.m[i] + LIGHT(x - 1, yT, z - 1, i) + LIGHT(x, yT, z - 1, i)) * cLight;
		face.l1m[i] = (LIGHT(x - 1, yT, z, i) + l.m[i] + LIGHT(x - 1, yT, z + 1, i) + LIGHT(x, yT, z + 1, i)) * cLight;
		face.l2m[i] = (LIGHT(x + 1, yT, z, i) + l.m[i] + LIGHT(x + 1, yT, z + 1, i) + LIGHT(x, yT, z + 1, i)) * cLight;
		face.l3m[i] = (LIGHT(x + 1, yT, z, i) + l.m[i] + LIGHT(x + 1, yT, z - 1, i) + LIGHT(x, yT, z - 1, i)) * cLight;
	}
}
//Bottom-------------------------
void  Chunk::getLightBottom(int x, int yB, int z, LightFace& face) {
	Vec4M l(getFastLight(x, yB, z));
	for (size_t i = 0; i < 4; i++) {
		face.l0m[i] = (LIGHT(x - 1, yB, z, i) + l.m[i] + LIGHT(x - 1, yB, z - 1, i) + LIGHT(x, yB, z - 1, i)) * cLight;
		face.l2m[i] = (LIGHT(x - 1, yB, z, i) + l.m[i] + LIGHT(x - 1, yB, z + 1, i) + LIGHT(x, yB, z + 1, i)) * cLight;
		face.l1m[i] = (LIGHT(x + 1, yB, z, i) + l.m[i] + LIGHT(x + 1, yB, z + 1, i) + LIGHT(x, yB, z + 1, i)) * cLight;
		face.l3m[i] = (LIGHT(x + 1, yB, z, i) + l.m[i] + LIGHT(x + 1, yB, z - 1, i) + LIGHT(x, yB, z - 1, i)) * cLight;
	}
}
//Right-------------------------
void  Chunk::getLightRight(int xR, int y, int z, LightFace& face) {

	Vec4M l(getFastLight(xR, y, z));
	for (size_t i = 0; i < 4; i++) {
		face.l0m[i] = (LIGHT(xR, y - 1, z - 1, i) + l.m[i] + LIGHT(xR, y, z - 1, i) + LIGHT(xR, y - 1, z, i)) * cLight;
		face.l1m[i] = (LIGHT(xR, y + 1, z - 1, i) + l.m[i] + LIGHT(xR, y, z - 1, i) + LIGHT(xR, y + 1, z, i)) * cLight;
		face.l2m[i] = (LIGHT(xR, y + 1, z + 1, i) + l.m[i] + LIGHT(xR, y, z + 1, i) + LIGHT(xR, y + 1, z, i)) * cLight;
		face.l3m[i] = (LIGHT(xR, y - 1, z + 1, i) + l.m[i] + LIGHT(xR, y, z + 1, i) + LIGHT(xR, y - 1, z, i)) * cLight;
	}

}
//Left-------------------------
void  Chunk::getLightLeft(int xL, int y, int z, LightFace& face) {
	Vec4M l(getFastLight(xL, y, z));
	for (size_t i = 0; i < 4; i++) {
		face.l0m[i] = (LIGHT(xL, y - 1, z - 1, i) + l.m[i] + LIGHT(xL, y, z - 1, i) + LIGHT(xL, y - 1, z, i)) * cLight;
		face.l1m[i] = (LIGHT(xL, y + 1, z + 1, i) + l.m[i] + LIGHT(xL, y, z + 1, i) + LIGHT(xL, y + 1, z, i)) * cLight;
		face.l2m[i] = (LIGHT(xL, y + 1, z - 1, i) + l.m[i] + LIGHT(xL, y, z - 1, i) + LIGHT(xL, y + 1, z, i)) * cLight;
		face.l3m[i] = (LIGHT(xL, y - 1, z + 1, i) + l.m[i] + LIGHT(xL, y, z + 1, i) + LIGHT(xL, y - 1, z, i)) * cLight;
	}
}
//Forward-------------------------
void  Chunk::getLightForward(int x, int y, int zF, LightFace& face) {
	Vec4M l(getFastLight(x, y, zF));
	for (size_t i = 0; i < 4; i++) {
		face.l0m[i] = (LIGHT(x - 1, y - 1, zF, i) + l.m[i] + LIGHT(x, y - 1, zF, i) + LIGHT(x - 1, y, zF, i)) * cLight;
		face.l1m[i] = (LIGHT(x + 1, y + 1, zF, i) + l.m[i] + LIGHT(x, y + 1, zF, i) + LIGHT(x + 1, y, zF, i)) * cLight;
		face.l2m[i] = (LIGHT(x - 1, y + 1, zF, i) + l.m[i] + LIGHT(x, y + 1, zF, i) + LIGHT(x - 1, y, zF, i)) * cLight;
		face.l3m[i] = (LIGHT(x + 1, y - 1, zF, i) + l.m[i] + LIGHT(x, y - 1, zF, i) + LIGHT(x + 1, y, zF, i)) * cLight;
	}
}
//Back-------------------------
void  Chunk::getLightBack(int x, int y, int zB, LightFace& face) {
	Vec4M l(getFastLight(x, y, zB));
	for (size_t i = 0; i < 4; i++) {
		face.l0m[i] = (LIGHT(x - 1, y - 1, zB, i) + l.m[i] + LIGHT(x, y - 1, zB, i) + LIGHT(x - 1, y, zB, i)) * cLight;
		face.l1m[i] = (LIGHT(x - 1, y + 1, zB, i) + l.m[i] + LIGHT(x, y + 1, zB, i) + LIGHT(x - 1, y, zB, i)) * cLight;
		face.l2m[i] = (LIGHT(x + 1, y + 1, zB, i) + l.m[i] + LIGHT(x, y + 1, zB, i) + LIGHT(x + 1, y, zB, i)) * cLight;
		face.l3m[i] = (LIGHT(x + 1, y - 1, zB, i) + l.m[i] + LIGHT(x, y - 1, zB, i) + LIGHT(x + 1, y, zB, i)) * cLight;
	}
}

inline void push_back(iGeometry<VoxelVertex>& convex, const glm::vec3& pos, const glm::vec2& uv, const glm::vec4& light) {
	convex.push_back(VoxelVertex(pos, uv, light));
}
void Chunk::buildBox(ShellGeometry& mesh, Voxel voxel,size_t x,size_t y,size_t z) {
	float uvsize = VoxelPack::get()->getNormalizeSizeVoxel();

	LightFace lFace;
	byte drawGroup = VoxelPack::get(voxel).drawGroup;
	//top
	if (isFree(x, y + 1, z, drawGroup)) {
		getLightTop(x, y + 1, z, lFace);
		const glm::vec2& uv = VoxelPack::get(voxel, Side::top);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y), lFace.l3);
	}

	if (isFree(x, y - 1, z, drawGroup)) {
		getLightBottom(x, y - 1, z, lFace);
		const glm::vec2& uv = VoxelPack::get(voxel, Side::bottom);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l3);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
	}

	if (isFree(x + 1, y, z, drawGroup)) {
		getLightRight(x + 1, y, z, lFace);
		const glm::vec2& uv = VoxelPack::get(voxel, Side::right);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y), lFace.l3);
	}

	if (isFree(x - 1, y, z, drawGroup)) {
		getLightLeft(x - 1, y, z, lFace);
		const glm::vec2& uv = VoxelPack::get(voxel, Side::left);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l3);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
	}

	if (isFree(x, y, z + 1, drawGroup)) {
		getLightForward(x, y, z + 1, lFace);
		const glm::vec2& uv = VoxelPack::get(voxel, Side::front);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l3);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
	}

	if (isFree(x, y, z - 1, drawGroup)) {
		getLightBack(x, y, z - 1, lFace);
		const glm::vec2& uv = VoxelPack::get(voxel, Side::back);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y), lFace.l3);
	}
}

void Chunk::buildÑrossroad(ShellGeometry&mesh,Voxel voxel, size_t x, size_t y, size_t z) {
	float uvsize = VoxelPack::get()->getNormalizeSizeVoxel();

	LightFace lFace;
	byte drawGroup = VoxelPack::get(voxel).drawGroup;
	const glm::vec2& uv = VoxelPack::get(voxel, Side::top);
	glm::vec4 l = 
		glm::vec4(
		  getFastLight(x, y+1, z) 
		+ getFastLight(x, y, z+1) 
		+ getFastLight(x, y, z + 1)
		+ getFastLight(x+1, y, z)
		+ getFastLight(x, y-1, z)
		+ getFastLight(x-1,y, z)
		+ getFastLight(x, y, z-1))*cLight;

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			l);//1
	push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	l);//3
	push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			l);  //3b
	push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),						l);					  //1b
	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize),			l);//1
	push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	l);//3
	push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y),			l);  //3b
	push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y),						l);
}

void Chunk::buildLiquid(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {

	if (voxel.e.m1 == 0) {
		buildBox(mesh,voxel, x, y, z);
	}
	else {
		buildBox(mesh, voxel, x, y, z);
		LOG("Half-Water\n");
	}
}

void Chunk::buildMesh() {	
	mesh.vertices.clear();
	mesh.indices.clear();
	sort_vox.clear();
	for (size_t y = 0; y < CHUNK_H; y++) {
		for (size_t z = 0; z < CHUNK_D; z++) {
			for (size_t x = 0; x < CHUNK_W; x++) {
				Voxel v = voxs(x, y, z);
				if (VoxelPack::isRender(v) == 0) continue;
				if (VoxelPack::isOpaque(v)) {
					if (v.e.id == vox::grass) buildÑrossroad(mesh, v, x, y, z);
					else  buildBox(mesh,v, x, y, z);
				}
				else sort_vox.push_back(glm::uvec3(x,y,z));
			}
		}
	}
	mesh.needUpBuffer = 1;
	modified = 0;
}

void  Chunk::buildSortMesh(const glm::ivec3& posView) {
	if (curPosView == posView) return;

	curPosView = posView;
	//Math distance
	for (size_t i = 0; i < sort_vox.size(); i++) {
		sort_vox[i].d = 
			abs((int)sort_vox[i].pos.x - curPosView.x) + 
			abs((int)sort_vox[i].pos.y - curPosView.y) + 
			abs((int)sort_vox[i].pos.z - curPosView.z);
	}
	//Sort
	sort(sort_vox.begin(), sort_vox.end(), [](const SortableVoxel& l, const SortableVoxel& r) {return l.d > r.d; });
	//Creating mesh
	for (size_t i = 0; i < sort_vox.size(); i++) {
		glm::uvec3 p = sort_vox[i].pos;
		Voxel v = voxs(p);

		if(v.e.id==vox::water)		buildLiquid(	sort_mesh, v, p.x, p.y, p.z);
		if (v.e.id == vox::grass)	buildÑrossroad(	sort_mesh, v, p.x, p.y, p.z);
		else						buildBox(		sort_mesh,v, p.x, p.y, p.z);

	}
	//Flags
	sort_mesh.needUpBuffer = 1;
}

void Chunk::drawOpaqueMesh(const Shader& shader) {
	if (mesh.needUpBuffer)
	{
		mesh.needUpBuffer = 0;
		mesh.saveInBuffer();
	}
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(global_) + glm::vec3(0.5f)));
	mesh.draw();
}

void Chunk::drawSortMesh(const Shader& shader) {
	if (sort_mesh.needUpBuffer)
	{
		sort_mesh.needUpBuffer = 0;
		sort_mesh.saveInBuffer();
	}
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(global_) + glm::vec3(0.5f)));
	sort_mesh.draw();
}
