#include "Chunk.h"

//CHunk-------------------------------------
/// <summary>
/// 
/// </summary>

bool gChunk::setVoxel(Voxel voxel, glm::uvec3 coord) {
	coord = coord - bg_vox;
	Voxel* voxel_ =voxs.get(coord);
	//существует ли воксель
	if (voxel_ == 0) return 0;
	//идентичен ли он данному
	if (*voxel_ == voxel)return 1;
	//Определям что потребуется перестроить
	if (VoxPack::isAlpha(voxel)&&((VoxPack::isAlpha(*voxel_) || VoxPack::isRender(*voxel_) == 0)) ) flag.modifyAlpha();
	else {
		flag.modify();
		closes.modify(coord.x, coord.z);
	}
	//Физическая оптимизация
	size_t index = voxs.ind(coord);
	if (VoxPack::isActive(*voxel_)) {
		//удаляем физически активный воксель из вектора
		if (VoxPack::isActive(voxel) == 0) {
			nonStatic.voxs_.erase(index);
		}
	}
	//добавляем физически активный воксель в вектора
	else if (VoxPack::isActive(voxel)) {
		nonStatic.voxs_.insert(index);
	}
	//nonStatic.log();
	*voxel_ = voxel;
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
#define cLight 1.f/70.f
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
Chunk::LightFace lFace;
void Chunk::buildTopFace(ShellGeometry& mesh, int x,int y,int z,byte drawGroup,Voxel voxel,float uvsize) {
	if (isFree(x, y+1, z, drawGroup)) {
		getLightTop(x, y+1, z, lFace);
		const glm::vec2& uv = VoxPack::get(voxel, Side::top);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y), lFace.l3);
	}
}
void Chunk::buildBottomFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
	if (isFree(x, y-1, z, drawGroup)) {
		getLightBottom(x, y-1, z, lFace);
		const glm::vec2& uv = VoxPack::get(voxel, Side::bottom);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l3);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
	}
}
void Chunk::buildRightFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
	if (isFree(x+1, y, z, drawGroup)) {
		getLightRight(x+1, y, z, lFace);
		const glm::vec2& uv = VoxPack::get(voxel, Side::right);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y), lFace.l3);
	}
}
void Chunk::buildLeftFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
	if (isFree(x-1, y, z, drawGroup)) {
		getLightLeft(x-1, y, z, lFace);
		const glm::vec2& uv = VoxPack::get(voxel, Side::left);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l3);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
	}
}
void Chunk::buildFrontFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
	if (isFree(x, y, z+1, drawGroup)) {
		getLightForward(x, y, z + 1, lFace);
		const glm::vec2& uv = VoxPack::get(voxel, Side::front);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l3);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z + 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
	}
}
void Chunk::buildBackFace(ShellGeometry& mesh, int x, int y, int z, byte drawGroup, Voxel voxel, float uvsize) {
	if (isFree(x, y, z-1, drawGroup)) {
		getLightBack(x, y, z-1, lFace);
		const glm::vec2& uv = VoxPack::get(voxel, Side::back);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y), lFace.l0);
		push_back(mesh, glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x + uvsize, uv.y + uvsize), lFace.l1);
		push_back(mesh, glm::vec3(x + 0.5f, y + 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y + uvsize), lFace.l2);
		push_back(mesh, glm::vec3(x + 0.5f, y - 0.5f, z - 0.5f), glm::vec2(uv.x, uv.y), lFace.l3);
	}
}
void Chunk::buildBox(ShellGeometry& mesh, Voxel voxel,size_t x,size_t y,size_t z) {
	float uvsize = VoxPack::get()->getNormalizeSizeVoxel();
	byte dGroup = VoxPack::get(voxel).drawGroup;
	buildTopFace(		mesh,x, y, z, dGroup, voxel, uvsize);
	buildBottomFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	buildRightFace(		mesh,x, y, z, dGroup, voxel, uvsize);
	buildLeftFace(		mesh,x, y, z, dGroup, voxel, uvsize);
	buildFrontFace(		mesh,x, y, z, dGroup, voxel, uvsize);
	buildBackFace(		mesh,x, y, z, dGroup, voxel, uvsize);
}
void Chunk::buildSortBox(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	float uvsize = VoxPack::get()->getNormalizeSizeVoxel();
	byte dGroup = VoxPack::get(voxel).drawGroup;
	bool isR = posView_.x > (int)x;
	bool isT = posView_.y > (int)y;
	bool isF = posView_.z > (int)z;

	if (isR == 0)	buildRightFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	else			buildLeftFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	if (isT == 0)	buildTopFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	else			buildBottomFace(mesh,x, y, z, dGroup, voxel, uvsize);
	if (isF == 0)	buildFrontFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	else			buildBackFace(	mesh,x, y, z, dGroup, voxel, uvsize);

	if (isR)		buildRightFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	else			buildLeftFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	if (isT)		buildTopFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	else			buildBottomFace(mesh,x, y, z, dGroup, voxel, uvsize);
	if (isF)		buildFrontFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	else			buildBackFace(	mesh,x, y, z, dGroup, voxel, uvsize);

}

void Chunk::buildСrossroad(ShellGeometry&mesh,Voxel voxel, size_t x, size_t y, size_t z) {
	float uvsize = VoxPack::get()->getNormalizeSizeVoxel();

	byte drawGroup = VoxPack::get(voxel).drawGroup;
	const glm::vec2& uv = VoxPack::get(voxel, Side::top);
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
	
	if (VoxPack::isSourceLiquid(voxel)) {
		buildSortBox(mesh,voxel, x, y, z);
	}
	else {
		buildSortBox(mesh, voxel, x, y, z);
		LOG("H-W\n");
	}
}

void Chunk::buildMesh() {	
	mesh_.vertices.clear();
	mesh_.indices.clear();
	sort_vox.clear();
	for(size_t i=0;i<voxs.size();i++){
		if (VoxPack::isRender(voxs[i]) == 0) continue;
		glm::uvec3 pos(voxs.coord(i));
		if (VoxPack::isAlpha(voxs[i])) sort_vox.push_back(pos);
		else {
			if (voxs[i].e.id == vox::grass) buildСrossroad(mesh_, voxs[i], pos.x, pos.y, pos.z);
			else  buildBox(mesh_, voxs[i], pos.x, pos.y, pos.z);
		}
	}
	mesh_.needUpBuffer = 1;
	flag.modified = 0;
	flag.modifyAlpha();
}

void  Chunk::buildSortMesh(glm::ivec3 pos) {
	pos = pos- glm::ivec3(bg_vox);
	if (posView_  == pos && flag.isModifiedAlpha()==0) return;
	flag.modifiedAlpha = 0;
	posView_ = pos;
	//LOG("UP\n");
	//Math distance
	for (size_t i = 0; i < sort_vox.size(); i++) {
		sort_vox[i].d = (
			abs((int)sort_vox[i].pos.x - posView_.x) +
			abs((int)sort_vox[i].pos.y - posView_.y) +
			abs((int)sort_vox[i].pos.z - posView_.z));
	}
	//Sort
	sort(sort_vox.begin(), sort_vox.end(), [](const SortableVoxel& l, const SortableVoxel& r) {return l.d > r.d; });
	//Creating mesh
	mesh_sort.vertices.clear();
	mesh_sort.indices.clear();
	for (size_t i = 0; i < sort_vox.size(); i++) {
		const glm::uvec3& p = sort_vox[i].pos;
		Voxel v = voxs(p);

		if(VoxPack::isLiquid(v))	buildLiquid(mesh_sort, v, p.x, p.y, p.z);
		else						buildSortBox(mesh_sort,v, p.x, p.y, p.z);

	}
	//Flags
	mesh_sort.needUpBuffer = 1;
}

void Chunk::drawOpaqueMesh(const Shader& shader) {
	if (mesh_.needUpBuffer)
	{
		mesh_.needUpBuffer = 0;
		mesh_.saveInBuffer();
	}
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(bg_vox) + glm::vec3(0.5f)));
	mesh_.draw();
}

void Chunk::drawSortMesh(const Shader& shader) {
	if (mesh_sort.needUpBuffer)
	{
		mesh_sort.needUpBuffer = 0;
		mesh_sort.saveInBuffer();
	}
	shader.uniform("model", glm::translate(glm::mat4(1.f), glm::vec3(bg_vox) + glm::vec3(0.5f)));
	mesh_sort.draw();
}
