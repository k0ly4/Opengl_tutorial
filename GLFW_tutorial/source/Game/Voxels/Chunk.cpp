#include "Chunk.h"

//CHunk-------------------------------------
/// <summary>
/// 
/// </summary>

void gChunk::setVoxelLocal(Voxel voxel, const glm::uvec3& coord) {
	//if (voxel.e.id == vox::water)LOG("setVoxelLocal", VoxPack::isActive(voxel));
	size_t index = voxs.ind(coord);
	Voxel dest = voxs[index];
	//идентичен ли он данному
	if (dest == voxel)return;
	voxs[index] = voxel;
	//Определям что потребуется перестроить
	if (VoxPack::isAlpha(voxel) && ((VoxPack::isAlpha(dest) || VoxPack::isRender(dest) == 0))) {
		upSortVox();
		flag.modifyAlpha();
		closes.modifyAlpha(coord.x, coord.z);
	}
	else {
		flag.modify();
		closes.modify(coord.x, coord.z);
	}
	//Физическая оптимизация
	//if (voxel.e.id == vox::water)LOG("Water:if == %d\n", VoxPack::isActive(voxel));
	if (VoxPack::isActive(voxel)) {
		nonStatic.insert(index);
	}
	else if (VoxPack::isActive(dest)) {
		glm::ivec3 off[6] = { 
			glm::ivec3(-1, 0, 0),
			glm::ivec3(1, 0, 0),
			glm::ivec3(0, -1, 0),
			glm::ivec3(0, 1, 0) ,
			glm::ivec3(0, 0, -1) ,
			glm::ivec3(0, 0, 1) 
		};
		for (size_t i = 0; i < 6; i++) {
			const Voxel* vox = voxs.get(glm::ivec3(coord) + off[i]);
			if (vox && VoxPack::isActive(*vox)) nonStatic.insert(voxs.ind(glm::ivec3(coord) + off[i]));
		}
	}
}
#define cLight (1.f/80.f)
//Top-------------------------
void MeshChunk::lightFaceXZ(int x, int y, int z, Vec4Array* face) {
	ColorU ls[9] = { 
		LIGHT(x - 1, y, z - 1),	LIGHT(x, y, z-1),	LIGHT(x+1, y, z-1),
		LIGHT(x - 1, y, z    ),	LIGHT(x, y, z),		LIGHT(x + 1, y, z),
		LIGHT(x - 1, y, z + 1),	LIGHT(x, y, z + 1),	LIGHT(x + 1, y, z + 1)
	};
	for (size_t i = 0; i < 4; i++) {
		face[0].m[i] = (ls[0].m[i] +ls[4].m[i] +ls[3].m[i] +   ls[1].m[i]) * cLight;
		face[1].m[i] = (ls[6].m[i] +ls[4].m[i] +ls[3].m[i] +   ls[7].m[i]) * cLight;
		face[2].m[i] = (ls[8].m[i] +ls[4].m[i] +ls[5].m[i] +   ls[7].m[i]) * cLight;
		face[3].m[i] = (ls[2].m[i] +ls[4].m[i] +ls[5].m[i] +   ls[1].m[i]) * cLight;
	}
}
//Right-------------------------
void  MeshChunk::lightFaceYZ(int x, int y, int z, Vec4Array* face) {
	ColorU ls[9] = {
		LIGHT(x, y-1, z - 1),		LIGHT(x, y, z - 1),	LIGHT(x, y+ 1, z - 1),
		LIGHT(x, y-1, z),			LIGHT(x, y, z),		LIGHT(x, y+ 1, z),
		LIGHT(x, y-1, z + 1),		LIGHT(x, y, z + 1),	LIGHT(x, y+ 1, z + 1)
	};
	for (size_t i = 0; i < 4; i++) {
		face[0].m[i] = (ls[0].m[i] + ls[4].m[i] + ls[1].m[i] + ls[3].m[i]) * cLight;
		face[1].m[i] = (ls[2].m[i] + ls[4].m[i] + ls[1].m[i] + ls[5].m[i]) * cLight;
		face[2].m[i] = (ls[8].m[i] + ls[4].m[i] + ls[7].m[i] + ls[5].m[i]) * cLight;
		face[3].m[i] = (ls[6].m[i] + ls[4].m[i] + ls[7].m[i] + ls[3].m[i]) * cLight;
	}
}
//Forward-------------------------
void  MeshChunk::lightFaceXY(int x, int y, int z, Vec4Array* face) {
	ColorU ls[9] = {
		LIGHT(x - 1, y - 1, z),			LIGHT(x- 1,y,z),		LIGHT(x- 1, y + 1,z),
		LIGHT(x	 ,	 y - 1, z),			LIGHT(x	 , y, z),		LIGHT(x	 , y + 1, z),
		LIGHT(x + 1, y - 1, z),			LIGHT(x+ 1,y,z),		LIGHT(x+ 1, y + 1,z)
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

void MeshChunk::buildTopFace(ShellGeometry& mesh, int x,int y,int z,Voxel voxel,float uvsize) {
	lightFaceXZ(x, y+1, z, lFace);
	glm::vec2 uv = VoxPack::get(voxel, Side::top);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x,		y + heights[0], z ),				glm::vec2(uv.x + uvsize, uv.y),				lFace[0].v);
	push_back(mesh, glm::vec3(x,		y + heights[1], z +  1.f),		glm::vec2(uv.x + uvsize, uv.y + uvsize),	lFace[1].v);
	push_back(mesh, glm::vec3(x + 1.f,	y + heights[2], z +  1.f), glm::vec2(uv.x, uv.y + uvsize),				lFace[2].v);
	push_back(mesh, glm::vec3(x + 1.f,	y + heights[3], z),		glm::vec2(uv.x, uv.y),						lFace[3].v);
}

void MeshChunk::buildBottomFace(ShellGeometry& mesh, int x, int y, int z,  Voxel voxel, float uvsize) {
	lightFaceXZ(x, y-1, z, lFace);
	glm::vec2 uv = VoxPack::get(voxel, Side::bottom);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x,		y, z ),				glm::vec2(uv.x, uv.y),						lFace[0].v);
	push_back(mesh, glm::vec3(x +  1.f, y, z  ),			glm::vec2(uv.x + uvsize, uv.y),				lFace[3].v);
	push_back(mesh, glm::vec3(x +  1.f, y, z + 1.f),		glm::vec2(uv.x + uvsize, uv.y + uvsize),	lFace[2].v);
	push_back(mesh, glm::vec3(x ,		y, z + 1.f),		glm::vec2(uv.x, uv.y + uvsize),				lFace[1].v);
}

void MeshChunk::buildRightFace(ShellGeometry& mesh, int x, int y, int z,Voxel voxel, float uvsize) {
	lightFaceYZ(x+1, y, z, lFace);
	glm::vec2 uv = VoxPack::get(voxel, Side::right);

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x + 1.f, y,				z),				glm::vec2(uv.x + uvsize, uv.y),				lFace[0].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[3],	z),				glm::vec2(uv.x + uvsize, uv.y + uvsize),	lFace[1].v);
	push_back(mesh, glm::vec3(x + 1.f, y + heights[2],	z + 1.f),		glm::vec2(uv.x, uv.y + uvsize),				lFace[2].v);
	push_back(mesh, glm::vec3(x + 1.f, y,				z + 1.f),		glm::vec2(uv.x, uv.y),						lFace[3].v);
	
}

void MeshChunk::buildLeftFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel, float uvsize) {
	
		lightFaceYZ(x-1, y, z, lFace);
		glm::vec2 uv = VoxPack::get(voxel, Side::left);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x, y,				z),				glm::vec2(uv.x, uv.y),						lFace[0].v);
		push_back(mesh, glm::vec3(x, y,				z + 1.f),		glm::vec2(uv.x + uvsize, uv.y),				lFace[3].v);
		push_back(mesh, glm::vec3(x, y + heights[1],z + 1.f),		glm::vec2(uv.x + uvsize, uv.y + uvsize),	lFace[2].v);
		push_back(mesh, glm::vec3(x, y + heights[0],z),				glm::vec2(uv.x, uv.y + uvsize),				lFace[1].v);
}

void MeshChunk::buildFrontFace(ShellGeometry& mesh, int x, int y, int z, Voxel voxel, float uvsize) {
		lightFaceXY(x, y, z + 1, lFace);
		glm::vec2 uv = VoxPack::get(voxel, Side::front);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x, y,						z + 1.f),	glm::vec2(uv.x, uv.y),						lFace[0].v);
		push_back(mesh, glm::vec3(x + 1.f, y,				z + 1.f),	glm::vec2(uv.x + uvsize, uv.y),				lFace[1].v);
		push_back(mesh, glm::vec3(x + 1.f, y + heights[2],	z + 1.f),	glm::vec2(uv.x + uvsize, uv.y + uvsize),	lFace[2].v);
		push_back(mesh, glm::vec3(x, y + heights[1],		z + 1.f),	glm::vec2(uv.x, uv.y + uvsize),				lFace[3].v);
}

void MeshChunk::buildBackFace(ShellGeometry& mesh, int x, int y, int z,Voxel voxel, float uvsize) {
		lightFaceXY(x, y, z-1, lFace);
		glm::vec2 uv = VoxPack::get(voxel, Side::back);

		mesh.pushIndices(mesh.vertices.size());
		push_back(mesh, glm::vec3(x      , y			, z), glm::vec2(uv.x + uvsize, uv.y),			lFace[0].v);
		push_back(mesh, glm::vec3(x      , y + heights[0], z), glm::vec2(uv.x + uvsize, uv.y + uvsize),	lFace[3].v);
		push_back(mesh, glm::vec3(x + 1.f, y + heights[3], z), glm::vec2(uv.x, uv.y + uvsize),			lFace[2].v);
		push_back(mesh, glm::vec3(x + 1.f, y			, z), glm::vec2(uv.x, uv.y),						lFace[1].v);
}

void MeshChunk::buildBox(ShellGeometry& mesh, Voxel voxel,size_t x,size_t y,size_t z) {
	heights[0] = heights[1] = heights[2] = heights[3] = 1.f;
	float uvsize = VoxPack::get()->getNormalizeSizeVoxel();
	byte dGroup = VoxPack::get(voxel).drawGroup;
	buildTopFace(		mesh,x, y, z, dGroup, voxel, uvsize);
	buildBottomFace(	mesh,x, y, z, dGroup, voxel, uvsize);
	buildRightFace(		mesh,x, y, z, dGroup, voxel, uvsize);
	buildLeftFace(		mesh,x, y, z, dGroup, voxel, uvsize);
	buildFrontFace(		mesh,x, y, z, dGroup, voxel, uvsize);
	buildBackFace(		mesh,x, y, z, dGroup, voxel, uvsize);
}
void MeshChunk::buildSortBox(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	heights[0] = heights[1] = heights[2] = heights[3] = 1.f;
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

void MeshChunk::buildLiquidBox(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	float uvsize = VoxPack::get()->getNormalizeSizeVoxel();
	byte dGroup = VoxPack::get(voxel).drawGroup;
	heights[0] = heights[1] = heights[2] = heights[3] = (float)voxel.e.m1/(float)(VoxPack::maxConcLiquid+1);
	Voxel targ = VOXEL(x, y+1, z);
	if (targ.e.id == voxel.e.id) {
		heights[0] = heights[1] = heights[2] = heights[3] = 1.f;
	}
	else { 
	targ = VOXEL(x + 1, y, z);
	if (targ.e.id == voxel.e.id && targ.e.m1> voxel.e.m1) {
		heights[2] = (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1);
		heights[3] = (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1);
	}
	targ = VOXEL(x - 1, y, z);
	if (targ.e.id == voxel.e.id && targ.e.m1 > voxel.e.m1) {
		heights[0] = (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1);
		heights[1] = (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1);
	}
	targ = VOXEL(x, y, z+1);
	if (targ.e.id == voxel.e.id && targ.e.m1 > voxel.e.m1) {
		heights[1] = (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1);
		heights[2] = (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1);
	}
	targ = VOXEL(x, y, z-1);
	if (targ.e.id == voxel.e.id && targ.e.m1 > voxel.e.m1) {
		heights[0] = (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1);
		heights[3] = (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1);
	}
	targ = VOXEL(x - 1, y, z - 1);
	if (targ.e.id == voxel.e.id && targ.e.m1 > voxel.e.m1)
		heights[0] = std::max(heights[0], (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1));
	targ = VOXEL(x - 1, y, z + 1);
	if (targ.e.id == voxel.e.id && targ.e.m1 > voxel.e.m1)
		heights[1] = std::max(heights[1], (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1));
	targ = VOXEL(x+1, y, z + 1);
	if (targ.e.id == voxel.e.id && targ.e.m1 > voxel.e.m1) 
		heights[2] = std::max(heights[2],(float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1));
	targ = VOXEL(x + 1, y, z-1);
	if (targ.e.id == voxel.e.id && targ.e.m1 > voxel.e.m1) 
		heights[3] = std::max(heights[3], (float)targ.e.m1 / (float)(VoxPack::maxConcLiquid + 1));
	}
	

	bool isR = posView_.x > (int)x;
	bool isT = posView_.y > (int)y;
	bool isF = posView_.z > (int)z;
	if (isR == 0)	buildRightFace(mesh, x, y, z, dGroup, voxel, uvsize);
	else			buildLeftFace(mesh, x, y, z, dGroup, voxel, uvsize);
	if (isT == 0)	buildTopFace(mesh, x, y, z, dGroup, voxel, uvsize);
	else			buildBottomFace(mesh, x, y, z, dGroup, voxel, uvsize);
	if (isF == 0)	buildFrontFace(mesh, x, y, z, dGroup, voxel, uvsize);
	else			buildBackFace(mesh, x, y, z, dGroup, voxel, uvsize);

	if (isR)		buildRightFace(mesh, x, y, z, dGroup, voxel, uvsize);
	else			buildLeftFace(mesh, x, y, z, dGroup, voxel, uvsize);
	if (isT)		buildTopFace(mesh, x, y, z, dGroup, voxel, uvsize);
	else			buildBottomFace(mesh, x, y, z, dGroup, voxel, uvsize);
	if (isF)		buildFrontFace(mesh, x, y, z, dGroup, voxel, uvsize);
	else			buildBackFace(mesh, x, y, z, dGroup, voxel, uvsize);

}
void MeshChunk::buildСrossroad(ShellGeometry&mesh,Voxel voxel, size_t x, size_t y, size_t z) {
	float uvsize = VoxPack::get()->getNormalizeSizeVoxel();

	byte drawGroup = VoxPack::get(voxel).drawGroup;
	const glm::vec2& uv = VoxPack::get(voxel, Side::top);
	glm::vec4 l = 
		glm::vec4(
		  getChannels(x, y+1, z) 
		+ getChannels(x, y, z+1)
		+ getChannels(x, y, z + 1)
		+ getChannels(x+1, y, z)
		+ getChannels(x, y-1, z)
		+ getChannels(x-1,y, z)
		+ getChannels(x, y, z-1))*cLight;

	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x		,	y + 1.f,	z  ), glm::vec2(uv.x, uv.y + uvsize),			l);//1
	push_back(mesh, glm::vec3(x + 1.f,	y + 1.f,	z + 1.f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	l);//3
	push_back(mesh, glm::vec3(x + 1.f,	y	,		z + 1.f), glm::vec2(uv.x + uvsize, uv.y),			l);  //3b
	push_back(mesh, glm::vec3(x		, y		, z ), glm::vec2(uv.x, uv.y),						l);					  //1b
	mesh.pushIndices(mesh.vertices.size());
	push_back(mesh, glm::vec3(x +  1.f, y +  1.f,	z), glm::vec2(uv.x, uv.y + uvsize),			l);//1
	push_back(mesh, glm::vec3(x	 , y +  1.f,		z + 1.f), glm::vec2(uv.x + uvsize, uv.y + uvsize),	l);//3
	push_back(mesh, glm::vec3(x	 , y  ,				z + 1.f), glm::vec2(uv.x + uvsize, uv.y),			l);  //3b
	push_back(mesh, glm::vec3(x + 1.f, y ,			z), glm::vec2(uv.x, uv.y),						l);
}

void MeshChunk::buildLiquid(ShellGeometry& mesh, Voxel voxel, size_t x, size_t y, size_t z) {
	buildLiquidBox(mesh, voxel, x, y, z);
}

void MeshChunk::buildMesh() {
	cBlock = 1;
	while (dBlock)std::this_thread::yield();
	//LOG("Bu");
	flag.modified = 0;
	mesh_.clear();
	s_vox.clear();
	for(size_t i=0;i<voxs.size();i++){
		if (VoxPack::isRender(voxs[i]) == 0) continue;
		glm::uvec3 pos(voxs.coord(i));
		if (VoxPack::isAlpha(voxs[i])) s_vox.push_back(i);
		else {
			if (voxs[i].e.id == vox::grass) buildСrossroad(mesh_, voxs[i], pos.x, pos.y, pos.z);
			else  buildBox(mesh_, voxs[i], pos.x, pos.y, pos.z);
		}
	}
	mesh_.needUpBuffer = 1;
	flag.modifyAlpha();
	//LOG("ild\n");
	cBlock = 0;
}

void  MeshChunk::buildSortMesh(glm::ivec3 pos) {
	
	pos = pos- glm::ivec3(bg_vox);
	if (((posView_  == pos) && (flag.isModifiedAlpha() == 0)) || flag.isModified()) return;
	cBlock = 1;
	while (dBlock)std::this_thread::yield();
	flag.modifiedAlpha = 0;

	posView_ = pos;
	//Math distance
	std::vector<SortableVoxel> s_vox = this->s_vox;
	for (size_t i = 0; i < s_vox.size(); i++) {
		glm::ivec3 pos(voxs.coord(s_vox[i].ind));
		s_vox[i].d = (
			abs(pos.x - posView_.x) +
			abs(pos.y - posView_.y) +
			abs(pos.z - posView_.z));
	}
	//Sort
	sort(s_vox.begin(), s_vox.end(), [](const SortableVoxel& l, const SortableVoxel& r) {return l.d > r.d; });
	//Creating mesh
	mesh_sort.clear();
	for (size_t i = 0; i < s_vox.size(); i++) {
		glm::ivec3 p(voxs.coord(s_vox[i].ind));
		Voxel v = voxs[s_vox[i].ind];

		if(VoxPack::isLiquid(v))	buildLiquid(mesh_sort, v, p.x, p.y, p.z);
		else						buildSortBox(mesh_sort,v, p.x, p.y, p.z);

	}
	//Flags
	mesh_sort.needUpBuffer = 1;
	cBlock = 0;
}

void MeshChunk::drawOpaqueMesh(const Shader& shader) {
	if ((flag.modified == 0) && mesh_.needUpBuffer)
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

void MeshChunk::drawSortMesh(const Shader& shader) {
	if ((flag.modified == 0) && (flag.modifiedAlpha == 0) && mesh_sort.needUpBuffer)
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
