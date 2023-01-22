#include "ChunkMesh.h"
#define cLight (1.f/80.f)
//Top-------------------------
void ChunkMeshBuilderDefault::lightFaceXZ(int x, int y, int z, Vec4Array* face) {
	ColorU ls[9] = {
		LIGHT(x - 1, y, z - 1),	LIGHT(x, y, z - 1),	LIGHT(x + 1, y, z - 1),
		LIGHT(x - 1, y, z),	LIGHT(x, y, z),		LIGHT(x + 1, y, z),
		LIGHT(x - 1, y, z + 1),	LIGHT(x, y, z + 1),	LIGHT(x + 1, y, z + 1)
	};
	for (size_t i = 0; i < 4; i++) {
		face[0].m[i] = (ls[0].m[i] + ls[4].m[i] + ls[3].m[i] + ls[1].m[i]) * cLight;
		face[1].m[i] = (ls[6].m[i] + ls[4].m[i] + ls[3].m[i] + ls[7].m[i]) * cLight;
		face[2].m[i] = (ls[8].m[i] + ls[4].m[i] + ls[5].m[i] + ls[7].m[i]) * cLight;
		face[3].m[i] = (ls[2].m[i] + ls[4].m[i] + ls[5].m[i] + ls[1].m[i]) * cLight;
	}
}
//Right-------------------------
void  ChunkMeshBuilderDefault::lightFaceYZ(int x, int y, int z, Vec4Array* face) {
	ColorU ls[9] = {
		LIGHT(x, y - 1, z - 1),		LIGHT(x, y, z - 1),	LIGHT(x, y + 1, z - 1),
		LIGHT(x, y - 1, z),			LIGHT(x, y, z),		LIGHT(x, y + 1, z),
		LIGHT(x, y - 1, z + 1),		LIGHT(x, y, z + 1),	LIGHT(x, y + 1, z + 1)
	};
	for (size_t i = 0; i < 4; i++) {
		face[0].m[i] = (ls[0].m[i] + ls[4].m[i] + ls[1].m[i] + ls[3].m[i]) * cLight;
		face[1].m[i] = (ls[2].m[i] + ls[4].m[i] + ls[1].m[i] + ls[5].m[i]) * cLight;
		face[2].m[i] = (ls[8].m[i] + ls[4].m[i] + ls[7].m[i] + ls[5].m[i]) * cLight;
		face[3].m[i] = (ls[6].m[i] + ls[4].m[i] + ls[7].m[i] + ls[3].m[i]) * cLight;
	}
}
//Forward-------------------------
void  ChunkMeshBuilderDefault::lightFaceXY(int x, int y, int z, Vec4Array* face) {
	ColorU ls[9] = {
		LIGHT(x - 1, y - 1, z),			LIGHT(x - 1,y,z),		LIGHT(x - 1, y + 1,z),
		LIGHT(x	 ,	 y - 1, z),			LIGHT(x	 , y, z),		LIGHT(x	 , y + 1, z),
		LIGHT(x + 1, y - 1, z),			LIGHT(x + 1,y,z),		LIGHT(x + 1, y + 1,z)
	};
	for (size_t i = 0; i < 4; i++) {
		face[0].m[i] = (ls[0].m[i] + ls[4].m[i] + ls[3].m[i] + ls[1].m[i]) * cLight;
		face[1].m[i] = (ls[6].m[i] + ls[4].m[i] + ls[3].m[i] + ls[7].m[i]) * cLight;
		face[2].m[i] = (ls[8].m[i] + ls[4].m[i] + ls[5].m[i] + ls[7].m[i]) * cLight;
		face[3].m[i] = (ls[2].m[i] + ls[4].m[i] + ls[5].m[i] + ls[1].m[i]) * cLight;
	}
}

inline void push_back(iGeometry<VoxelVertex>& convex, const glm::vec3& pos, const glm::vec2& uv, const glm::vec4& light) {
	convex.push_back(VoxelVertex(pos, uv, light));
}
Vec4Array lFace[4];

void ChunkMeshBuilderDefault::buildTopFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel) {
	lightFaceXZ(x, y + 1, z, lFace);
	glm::vec4 uv = VoxPack::get4(voxel, Side::top);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x, y + heights[0], z),				glm::vec2(uv.z, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x, y + heights[1], z + 1.f),			glm::vec2(uv.z, uv.w), lFace[1].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[2], z + 1.f),	glm::vec2(uv.x, uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[3], z),			glm::vec2(uv.x, uv.y), lFace[3].v);
}

void ChunkMeshBuilderDefault::buildBottomFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel) {
	lightFaceXZ(x, y - 1, z, lFace);
	glm::vec4 uv = VoxPack::get4(voxel, Side::bottom);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x, y, z),						glm::vec2(uv.x, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x + 1.f, y, z),				glm::vec2(uv.z, uv.y), lFace[3].v);
	push_back(mesh, glm::vec3(x + 1.f, y, z + 1.f),			glm::vec2(uv.z,	uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x, y, z + 1.f),				glm::vec2(uv.x,	uv.w), lFace[1].v);
}

void ChunkMeshBuilderDefault::buildRightFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel) {
	lightFaceYZ(x + 1, y, z, lFace);
	glm::vec4 uv = VoxPack::get4(voxel, Side::right);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x + 1.f, y, z),						glm::vec2(uv.z, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[3], z),			glm::vec2(uv.z, uv.w), lFace[1].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[2], z + 1.f),	glm::vec2(uv.x, uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x + 1.f, y, z + 1.f),					glm::vec2(uv.x, uv.y), lFace[3].v);

}

void ChunkMeshBuilderDefault::buildLeftFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel) {

	lightFaceYZ(x - 1, y, z, lFace);
	glm::vec4 uv = VoxPack::get4(voxel, Side::left);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x, y, z),						glm::vec2(uv.x, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x, y, z + 1.f),				glm::vec2(uv.z, uv.y), lFace[3].v);
	push_back(mesh, glm::vec3(x, y + heights[1], z + 1.f),	glm::vec2(uv.z,	uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x, y + heights[0], z),		glm::vec2(uv.x,	uv.w), lFace[1].v);
}

void ChunkMeshBuilderDefault::buildFrontFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel) {
	lightFaceXY(x, y, z + 1, lFace);
	glm::vec4 uv = VoxPack::get4(voxel, Side::front);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x, y, z + 1.f),					glm::vec2(uv.x, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x + 1.f, y, z + 1.f),				glm::vec2(uv.z, uv.y), lFace[1].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[2], z + 1.f),glm::vec2(uv.z, uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x, y + heights[1], z + 1.f),		glm::vec2(uv.x, uv.w), lFace[3].v);
}

void ChunkMeshBuilderDefault::buildBackFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel) {
	lightFaceXY(x, y, z - 1, lFace);
	glm::vec4 uv = VoxPack::get4(voxel, Side::back);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x, y, z),						glm::vec2(uv.z, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x, y + heights[0], z),		glm::vec2(uv.z, uv.w), lFace[3].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[3], z),	glm::vec2(uv.x, uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x + 1.f, y, z),				glm::vec2(uv.x, uv.y), lFace[1].v);
}


void ChunkMeshBuilderDefault::buildBox(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	heights[0] = heights[1] = heights[2] = heights[3] = 1.f;
	byte dGroup = VoxPack::get(voxel).drawGroup;
	buildTopFace(mesh, x, y, z, dGroup, voxel);
	buildBottomFace(mesh, x, y, z, dGroup, voxel);
	buildRightFace(mesh, x, y, z, dGroup, voxel);
	buildLeftFace(mesh, x, y, z, dGroup, voxel);
	buildFrontFace(mesh, x, y, z, dGroup, voxel	);
	buildBackFace(mesh, x, y, z, dGroup, voxel);
}
void ChunkMeshBuilderDefault::buildSortBox(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	heights[0] = heights[1] = heights[2] = heights[3] = 1.f;
	byte dGroup = VoxPack::get(voxel).drawGroup;
	bool isR = _this->posView_.x > (int)x;
	bool isT = _this->posView_.y > (int)y;
	bool isF = _this->posView_.z > (int)z;

	if (isR == 0)	buildRightFace(mesh, x, y, z,	dGroup, voxel);
	else			buildLeftFace(mesh, x, y, z,	dGroup, voxel);
	if (isT == 0)	buildTopFace(mesh, x, y, z,		dGroup, voxel);
	else			buildBottomFace(mesh, x, y, z,	dGroup, voxel);
	if (isF == 0)	buildFrontFace(mesh, x, y, z,	dGroup, voxel);
	else			buildBackFace(mesh, x, y, z,	dGroup, voxel);

	if (isR)		buildRightFace(mesh, x, y, z,	dGroup, voxel);
	else			buildLeftFace(mesh, x, y, z,	dGroup, voxel);
	if (isT)		buildTopFace(mesh, x, y, z,		dGroup, voxel);
	else			buildBottomFace(mesh, x, y, z,	dGroup, voxel);
	if (isF)		buildFrontFace(mesh, x, y, z,	dGroup, voxel);
	else			buildBackFace(mesh, x, y, z,	dGroup, voxel);

}

void ChunkMeshBuilderDefault::buildLiquidBox(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	byte dGroup = VoxPack::get(voxel).drawGroup;
	float mx = VoxPack::maxConcLiquid + 1;
	heights[0] = heights[1] = heights[2] = heights[3] = voxel.e.m1 / mx;
	Voxel trg = VOXEL(x, y + 1, z);
	//Top
	if (trg.e.id == voxel.e.id) {
		heights[0] = heights[1] = heights[2] = heights[3] = 1.f;
	}
	else {

		trg = VOXEL(x + 1, y + 1, z);
		if (trg.e.id == voxel.e.id)								 heights[2] = heights[3] = 1.f;
		else {
			trg = VOXEL(x + 1, y, z);
			if (trg.e.id == voxel.e.id && trg.e.m1 > voxel.e.m1) heights[2] = heights[3] = (float)trg.e.m1 / mx;
		}

		trg = VOXEL(x - 1, y + 1, z);
		if (trg.e.id == voxel.e.id)								 heights[0] = heights[1] = 1.f;
		else {
			trg = VOXEL(x - 1, y, z);
			if (trg.e.id == voxel.e.id && trg.e.m1 > voxel.e.m1) heights[0] = heights[1] = (float)trg.e.m1 / mx;
		}

		trg = VOXEL(x, y + 1, z + 1);
		if (trg.e.id == voxel.e.id)								 heights[1] = heights[2] = 1.f;
		else {
			trg = VOXEL(x, y, z + 1);
			if (trg.e.id == voxel.e.id && trg.e.m1 > voxel.e.m1) {
				float val = (float)trg.e.m1 / mx;
				heights[1] = std::max(heights[1], val);
				heights[2] = std::max(heights[2], val);
			}
		}
		trg = VOXEL(x, y + 1, z - 1);
		if (trg.e.id == voxel.e.id)								 heights[0] = heights[3] = 1.f;
		else {
			trg = VOXEL(x, y, z - 1);
			{
				if (trg.e.id == voxel.e.id && trg.e.m1 > voxel.e.m1) {
					float val = (float)trg.e.m1 / mx;
					heights[0] = std::max(heights[0], val);
					heights[3] = std::max(heights[3], val);
				}
			}
		}
		if (VOXEL(x - 1, y + 1, z - 1).e.id == voxel.e.id)heights[0] = 1.f;
		else {
			trg = VOXEL(x - 1, y, z - 1);
			if (trg.e.id == voxel.e.id && trg.e.m1 > voxel.e.m1) heights[0] = std::max(heights[0], (float)trg.e.m1 / mx);
		}
		if (VOXEL(x - 1, y + 1, z + 1).e.id == voxel.e.id)heights[1] = 1.f;
		else {
			trg = VOXEL(x - 1, y, z + 1);
			if (trg.e.id == voxel.e.id && trg.e.m1 > voxel.e.m1)heights[1] = std::max(heights[1], (float)trg.e.m1 / mx);
		}
		if (VOXEL(x + 1, y + 1, z + 1).e.id == voxel.e.id)heights[2] = 1.f;
		else {
			trg = VOXEL(x + 1, y, z + 1);
			if (trg.e.id == voxel.e.id && trg.e.m1 > voxel.e.m1)heights[2] = std::max(heights[2], (float)trg.e.m1 / mx);
		}
		if (VOXEL(x + 1, y + 1, z - 1).e.id == voxel.e.id)heights[3] = 1.f;
		else {
			trg = VOXEL(x + 1, y, z - 1);
			if (trg.e.id == voxel.e.id && trg.e.m1 > voxel.e.m1)heights[3] = std::max(heights[3], (float)trg.e.m1 / mx);
		}
	}


	bool isR = _this->posView_.x > (int)x;
	bool isT = _this->posView_.y > (int)y;
	bool isF = _this->posView_.z > (int)z;
	if (isR == 0)	buildRightFace(mesh, x, y, z,	dGroup, voxel);
	else			buildLeftFace(mesh, x, y, z,	dGroup, voxel);
	if (isT == 0)	buildTopFace(mesh, x, y, z,		dGroup, voxel);
	else			buildBottomFace(mesh, x, y, z,	dGroup, voxel);
	if (isF == 0)	buildFrontFace(mesh, x, y, z,	dGroup, voxel);
	else			buildBackFace(mesh, x, y, z,	dGroup, voxel);

	if (isR)		buildRightFace(mesh, x, y, z,	dGroup, voxel);
	else			buildLeftFace(mesh, x, y, z,	dGroup, voxel);
	if (isT)		buildTopFace(mesh, x, y, z,		dGroup, voxel);
	else			buildBottomFace(mesh, x, y, z,	dGroup, voxel);
	if (isF)		buildFrontFace(mesh, x, y, z,	dGroup, voxel);
	else			buildBackFace(mesh, x, y, z,	dGroup, voxel);

}
void ChunkMeshBuilderDefault::buildÑrossroad(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	
	byte drawGroup = VoxPack::get(voxel).drawGroup;
	glm::vec4 uv = VoxPack::get4(voxel, Side::top);
	glm::vec4 l =
		glm::vec4(
			getChannels(x, y + 1, z)
			+ getChannels(x, y, z + 1)
			+ getChannels(x, y, z + 1)
			+ getChannels(x + 1, y, z)
			+ getChannels(x, y - 1, z)
			+ getChannels(x - 1, y, z)
			+ getChannels(x, y, z - 1)) * cLight;

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x, y + 1.f, z),				glm::vec2(uv.x, uv.w), l);//1
	push_back(mesh, glm::vec3(x + 1.f, y + 1.f, z + 1.f),	glm::vec2(uv.z, uv.w), l);//3
	push_back(mesh, glm::vec3(x + 1.f, y, z + 1.f),			glm::vec2(uv.z, uv.y), l);  //3b
	push_back(mesh, glm::vec3(x, y, z),						glm::vec2(uv.x, uv.y), l);					  //1b
	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x + 1.f, y + 1.f, z), glm::vec2(uv.x, uv.w), l);//1
	push_back(mesh, glm::vec3(x, y + 1.f, z + 1.f), glm::vec2(uv.z, uv.w), l);//3
	push_back(mesh, glm::vec3(x, y, z + 1.f),		glm::vec2(uv.z, uv.y), l);  //3b
	push_back(mesh, glm::vec3(x + 1.f, y, z),		glm::vec2(uv.x, uv.y), l);
}

void ChunkMeshBuilderDefault::buildLiquid(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	buildLiquidBox(mesh, voxel, x, y, z);
}


void ChunkMeshBuilderDefault::buildOpaqueMesh() {
	_this->flag.modified = 0;
	_this->mesh_.clear();
	_this->s_vox.clear();
	for (size_t i = 0; i < _this->voxs.size(); i++) {
		if (VoxPack::isRender(_this->voxs[i]) == 0) continue;
		glm::uvec3 pos(_this->voxs.coord(i));
		if (VoxPack::isAlpha(_this->voxs[i])) _this->s_vox.push_back(i);
		else {
			if (_this->voxs[i].e.id == vox::grass) buildÑrossroad(_this->mesh_, _this->voxs[i], pos.x, pos.y, pos.z);
			else  buildBox(_this->mesh_, _this->voxs[i], pos.x, pos.y, pos.z);
		}
	}
	_this->mesh_.needUpBuffer = 1;
	_this->flag.modifyAlpha();
}

void ChunkMeshBuilderDefault::buildSortMesh() {
	_this->flag.modifiedAlpha = 0;
	//Math distance
	std::vector<SortableVoxel> s_vox = _this->s_vox;
	for (size_t i = 0; i < s_vox.size(); i++) {
		glm::ivec3 pos(_this->voxs.coord(s_vox[i].ind));
		s_vox[i].d = (
			abs(pos.x - _this->posView_.x) +
			abs(pos.y - _this->posView_.y) +
			abs(pos.z - _this->posView_.z));
	}
	//Sort
	sort(s_vox.begin(), s_vox.end(), [](const SortableVoxel& l, const SortableVoxel& r) {return l.d > r.d; });
	//Creating mesh
	_this->mesh_sort.clear();
	for (size_t i = 0; i < s_vox.size(); i++) {
		glm::ivec3 p(_this->voxs.coord(s_vox[i].ind));
		Voxel v = _this->voxs[s_vox[i].ind];

		if (VoxPack::isLiquid(v))	buildLiquid(_this->mesh_sort, v, p.x, p.y, p.z);
		else						buildSortBox(_this->mesh_sort, v, p.x, p.y, p.z);

	}
	//Flags
	_this->mesh_sort.needUpBuffer = 1;
}

std::shared_ptr<iChunkMeshBuilder> sChunkMesh::creator = 0;