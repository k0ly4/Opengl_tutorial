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

void ChunkMeshBuilderDefault::buildTopFace(iGeometry<VoxelVertex>& mesh, int x, int y, int z, Voxel voxel) {
	glm::vec4 uv = VoxPack::get4(voxel, Side::top);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x,	   y + heights[0], z),			glm::vec2(uv.z, uv.y),		  lFace[0].v);
	push_back(mesh, glm::vec3(x,	   y + heights[1], z + 1.f),	glm::vec2(uv.z, uv.w),		  lFace[1].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[2], z + 1.f),	glm::vec2(uv.x, uv.w),		  lFace[2].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[3], z),			glm::vec2(uv.x, uv.y),		  lFace[3].v);
}

void ChunkMeshBuilderDefault::buildBottomFace(iGeometry<VoxelVertex>& mesh, int x, int y, int z, Voxel voxel) {
	glm::vec4 uv = VoxPack::get4(voxel, Side::bottom);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x,		y,				z),					glm::vec2(uv.x, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x + 1.f,	y,				z),					glm::vec2(uv.z, uv.y), lFace[3].v);
	push_back(mesh, glm::vec3(x + 1.f,	y,				z + 1.f),			glm::vec2(uv.z,	uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x,		y,				z + 1.f),			glm::vec2(uv.x,	uv.w), lFace[1].v);
}

void ChunkMeshBuilderDefault::buildRightFace(iGeometry<VoxelVertex>& mesh, int x, int y, int z, Voxel voxel) {
	glm::vec4 uv = VoxPack::get4(voxel, Side::right);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x + 1.f, y,				z),					glm::vec2(uv.z, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[3],	z),					glm::vec2(uv.z, uv.w), lFace[1].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[2],	z + 1.f),			glm::vec2(uv.x, uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x + 1.f, y,				z + 1.f),			glm::vec2(uv.x, uv.y), lFace[3].v);

}

void ChunkMeshBuilderDefault::buildLeftFace(iGeometry<VoxelVertex>& mesh, int x, int y, int z, Voxel voxel) {
	glm::vec4 uv = VoxPack::get4(voxel, Side::left);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x,		y,				z),					glm::vec2(uv.x, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x,		y,				z + 1.f),			glm::vec2(uv.z, uv.y), lFace[3].v);
	push_back(mesh, glm::vec3(x,		y + heights[1], z + 1.f),			glm::vec2(uv.z,	uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x,		y + heights[0], z),					glm::vec2(uv.x,	uv.w), lFace[1].v);
}

void ChunkMeshBuilderDefault::buildFrontFace(iGeometry<VoxelVertex>& mesh, int x, int y, int z, Voxel voxel) {
	glm::vec4 uv = VoxPack::get4(voxel, Side::front);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x,		y,				z + 1.f),			glm::vec2(uv.x, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x + 1.f,	y,				z + 1.f),			glm::vec2(uv.z, uv.y), lFace[1].v);
	push_back(mesh, glm::vec3(x + 1.f,	y + heights[2], z + 1.f),			glm::vec2(uv.z, uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x,		y + heights[1], z + 1.f),			glm::vec2(uv.x, uv.w), lFace[3].v);
}

void ChunkMeshBuilderDefault::buildBackFace(iGeometry<VoxelVertex>& mesh, int x, int y, int z, Voxel voxel) {
	glm::vec4 uv = VoxPack::get4(voxel, Side::back);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x,		y,				z),					glm::vec2(uv.z, uv.y), lFace[0].v);
	push_back(mesh, glm::vec3(x,		y + heights[0], z),					glm::vec2(uv.z, uv.w), lFace[3].v);
	push_back(mesh, glm::vec3(x + 1.f,	y + heights[3], z),					glm::vec2(uv.x, uv.w), lFace[2].v);
	push_back(mesh, glm::vec3(x + 1.f,	y,				z),					glm::vec2(uv.x, uv.y), lFace[1].v);
}


void ChunkMeshBuilderDefault::buildBox(iGeometry<VoxelVertex>& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	setHeights(1.f);
	byte dGroup = VoxPack::get(voxel).drawGroup;
	buildTopFace(	mesh, x, y, z, dGroup, voxel);
	buildBottomFace(mesh, x, y, z, dGroup, voxel);
	buildRightFace(	mesh, x, y, z, dGroup, voxel);
	buildLeftFace(	mesh, x, y, z, dGroup, voxel);
	buildFrontFace(	mesh, x, y, z, dGroup, voxel);
	buildBackFace(	mesh, x, y, z, dGroup, voxel);
}
void ChunkMeshBuilderDefault::buildSortBox(iGeometry<VoxelVertex>& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	setHeights(1.f);
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
void ChunkMeshBuilderDefault::setLiquidHeights(Voxel voxel, size_t x, size_t y, size_t z) {
	//ÂÒÎË ÂÒÚ¸ Ò‚ÂıÛ
	if (VOXEL(x, y + 1, z).e.id == voxel.e.id) {
		setHeights(1.f);
		return;
	}
		float mx = 1.f / (float)(VoxPack::maxConcLiquid + 1);
		byte density = voxel.density();
		twobyte id = voxel.e.id;
		setHeights(density * mx);
		
		enum :size_t
		{
			lbt, bt, rbt,
			lt, rt,
			lft, ft, rft
		};
		Voxel xz_top[8] = {
			VOXEL(x - 1, y + 1, z - 1),	VOXEL(x, y + 1, z - 1),	VOXEL(x + 1, y + 1, z - 1),
			VOXEL(x - 1, y + 1, z),								VOXEL(x + 1, y + 1, z),
			VOXEL(x - 1, y + 1, z + 1),	VOXEL(x, y + 1, z + 1),	VOXEL(x + 1, y + 1, z + 1)
		};

		enum :size_t
		{
			lb, b, rb,
			l, r,
			lf, f, rf
		};
		Voxel xz[8] = {
			/*0*/VOXEL(x - 1, y,  z - 1), /*1*/VOXEL(x, y , z - 1),	/*2*/VOXEL(x + 1,   y, z - 1),
			/*3*/VOXEL(x - 1, y, z),								/*4*/VOXEL(x + 1, y, z),
			/*5*/VOXEL(x - 1, y, z + 1),/*6*/VOXEL(x, y, z + 1),	/*7*/VOXEL(x + 1, y, z + 1),
		};

		if (xz_top[rt].e.id == id)								 heights[2] = heights[3] = 1.f;
		else if ((xz[r].e.id == id) && (xz[r].density() > density)) {
			heights[2] = heights[3] = (float)xz[r].density() * mx;
		}

		if (xz_top[lt].e.id == id)								 heights[0] = heights[1] = 1.f;
		else if (xz[l].e.id == id && xz[l].density() > density) {
				heights[0] = heights[1] = (float)xz[l].density() * mx;
			}
		
		if (xz_top[ft].e.id == id)								 heights[1] = heights[2] = 1.f;
		else if ((xz[f].e.id == id) && (xz[f].density() > density)) {
				float val = (float)xz[f].e.m1 * mx;
				heights[1] = std::max(heights[1], val);
				heights[2] = std::max(heights[2], val);
			}
		
		if (xz_top[bt].e.id == id)								 heights[0] = heights[3] = 1.f;
		else if ((xz[b].e.id == id) && (xz[b].density() > density)) {
					float val = (float)xz[b].density() * mx;
					heights[0] = std::max(heights[0], val);
					heights[3] = std::max(heights[3], val);
				}
			
		
		if (xz_top[lbt].e.id == id)heights[0] = 1.f;
		else if ((xz[lb].e.id == id) && (xz[lb].density() > density)) {
				heights[0] = std::max(heights[0], (float)xz[lb].e.m1 * mx);
			}
		

		if (xz_top[lft].e.id == id)heights[1] = 1.f;
		else if ((xz[lf].e.id == id) && (xz[lf].density() > density)) {
				heights[1] = std::max(heights[1], (float)xz[lf].density() * mx);
			}

		if (xz_top[rft].e.id == id)heights[2] = 1.f;
		else if ((xz[rf].e.id == id) && (xz[rf].density() > density)) {
				heights[2] = std::max(heights[2], (float)xz[rf].density() * mx);
			}
		
		if (xz_top[rbt].e.id == id)heights[3] = 1.f;
		else if ((xz[rb] == id) && (xz[rb].density() > density)) {
			heights[3] = std::max(heights[3], (float)xz[rb].density() * mx);
			}
		
}
void ChunkMeshBuilderDefault::buildLiquidBox(iGeometry<VoxelVertex>& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	byte dGroup = VoxPack::get(voxel).drawGroup;
	setLiquidHeights(voxel, x, y, z);
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
void ChunkMeshBuilderDefault::build—rossroad(iGeometry<VoxelVertex>& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	
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
	push_back(mesh, glm::vec3(x + 1.f, y + 1.f, z),			glm::vec2(uv.x, uv.w), l);//1
	push_back(mesh, glm::vec3(x, y + 1.f, z + 1.f),			glm::vec2(uv.z, uv.w), l);//3
	push_back(mesh, glm::vec3(x, y, z + 1.f),				glm::vec2(uv.z, uv.y), l);  //3b
	push_back(mesh, glm::vec3(x + 1.f, y, z),				glm::vec2(uv.x, uv.y), l);
}

void ChunkMeshBuilderDefault::buildLiquid(iGeometry<VoxelVertex>& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	buildLiquidBox(mesh, voxel, x, y, z);
}
void ChunkMeshBuilderDefault::buildMeshIcon(Voxel vox) {
	
}

void ChunkMeshBuilderDefault::buildOpaqueMesh() {
	_this->flag.modified = 0;
	_this->mesh_.clear();
	for (size_t i = 0; i < _this->voxs.size(); i++) {
		Voxel v = _this->voxs[i];
		if (VoxPack::isRender(v) == 0) continue;
		glm::uvec3 pos(_this->voxs.coord(i));
		if (VoxPack::isAlpha(v) == 0)
		{
			if (_this->voxs[i].e.id == vox::grass)				build—rossroad(_this->mesh_, v, pos.x, pos.y, pos.z);
			else												buildBox(	   _this->mesh_, v,pos.x, pos.y, pos.z);
		}
	}
	_this->mesh_.needUpBuffer = 1;
	_this->flag.modifyAlpha();
}

void ChunkMeshBuilderDefault::buildSortMesh() {
	if (_this->flag.modifiedAlpha) {
		//LOG("upAlpha\n");
		_this->upAlphaVoxBuffer();
		_this->flag.modifiedAlpha = 0;
	}
	//cash distance
	for (size_t i = 0; i < _this->alpha_vox.size(); i++) {
		glm::ivec3 pos(_this->voxs.coord(_this->alpha_vox[i].ind));
		_this->alpha_vox[i].d = (
			abs(pos.x - _this->posView_.x) +
			abs(pos.y - _this->posView_.y) +
			abs(pos.z - _this->posView_.z));
	}
	//sort
	sort(_this->alpha_vox.begin(), _this->alpha_vox.end(), [](const SortableVoxel& l, const SortableVoxel& r) {return l.d > r.d; });
	//Creating mesh
	_this->mesh_sort.clear();
	for (size_t i = 0; i < _this->alpha_vox.size(); i++) {
		glm::ivec3 p(_this->voxs.coord(	_this->alpha_vox[i].ind));
		Voxel v =						_this->voxs[_this->alpha_vox[i].ind];

		if (VoxPack::isLiquid(v))	buildLiquidBox(_this->mesh_sort, v, p.x, p.y, p.z);
		else						buildSortBox(_this->mesh_sort, v, p.x, p.y, p.z);

	}
	//Flags
	_this->mesh_sort.needUpBuffer = 1;
}

std::shared_ptr<iChunkMeshBuilder> sChunkMesh::creator = 0;