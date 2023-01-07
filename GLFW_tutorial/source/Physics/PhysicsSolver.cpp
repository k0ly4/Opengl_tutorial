#include "PhysicsSolver.h"
#include "Scene/Scene.h"
#define E 0.03f
void PhysicsSolver::init(GlobalScene* scene_) {
	scene = scene_;
	world = &scene->sc3d.world;
}

void PhysicsSolver::step(Hitbox& hitbox) {
	
	float dt = rTime.step;
	//float linear_damping = hitbox.linear_damping;
 
	glm::vec3& pos = hitbox.position;
	glm::vec3& half = hitbox.halfsize;
	glm::vec3& vel = hitbox.velocity;
	float px = pos.x;
	float pz = pos.z;
	
	vel += gravity * dt * hitbox.getGravityScale();
	hitbox.grounded = false;
	
		
	
	ChunkSectorRender& chunks = world->chunks;
	pos.x += vel.x * dt;
	if (vel.x < 0.0) {
		for (size_t y = (size_t)floor(pos.y - half.y + E); y <= (size_t)floor(pos.y + half.y - E); y++) {
			for (size_t z = (size_t)floor(pos.z - half.z + E); z <= (size_t)floor(pos.z + half.z - E); z++) {
				size_t x = (size_t)floor(pos.x - half.x - E);

				if (chunks.isObstacle(glm::uvec3(x, y, z))) {
					vel.x = 0.f;
					pos.x = x + 1 + half.x + E;
					break;
				}
			}
		}
	}
	if (vel.x > 0.0) {
		for (size_t y = (size_t)floor(pos.y - half.y + E); y <= (size_t)floor(pos.y + half.y - E); y++) {
			for (size_t z = (size_t)floor(pos.z - half.z + E); z <= (size_t)floor(pos.z + half.z - E); z++) {
				size_t x = (size_t)floor(pos.x + half.x + E);
				if (chunks.isObstacle(glm::uvec3(x, y, z))) {
					vel.x = 0.f;
					pos.x = x - half.x - E;
					break;
				}
			}
		}
	}
	pos.z += vel.z * dt;
	if (vel.z < 0.0) {
		for (size_t y = (size_t)floor(pos.y - half.y + E); y <= (size_t)floor(pos.y + half.y - E); y++) {
			for (size_t x = (size_t)floor(pos.x - half.x + E); x <= (size_t)floor(pos.x + half.x - E); x++) {
				size_t z = (size_t)floor(pos.z - half.z - E);
				if (chunks.isObstacle(x, y, z)) {
					vel.z = 0.f;
					pos.z = z + 1 + half.z + E;
					break;
				}
			}
		}
	}

	if (vel.z > 0.0) {
		for (size_t y = (size_t)floor(pos.y - half.y + E); y <= floor(pos.y + half.y - E); y++) {
			for (size_t x = (size_t)floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
				size_t z = (size_t)floor(pos.z + half.z + E);
				if (chunks.isObstacle(x, y, z)) {
					vel.z = 0.f;
					pos.z = z - half.z - E;
					break;
				}
			}
		}
	}
	pos.y += vel.y * dt;
	if (vel.y < 0.0) {
		for (size_t x = (size_t)floor(pos.x - half.x + E); x <= (size_t)floor(pos.x + half.x - E); x++) {
			for (size_t z = (size_t)floor(pos.z - half.z + E); z <= (size_t)floor(pos.z + half.z - E); z++) {
				size_t y = (size_t)floor(pos.y - half.y - E);
				if (chunks.isObstacle(x, y, z)) {
					vel.y = 0.f;
					pos.y = y + 1 + half.y;
					//int f = DEFAULT_FRICTION;
					//vel.x *= max(0.0, 1.0 - dt * f);
					//vel.z *= max(0.0, 1.0 - dt * f);
					hitbox.grounded = true;
					goto velYComplete;
				}
			}
		}
	velYComplete:;
	}

	if (vel.y > 0.0) {
		for (int x = floor(pos.x - half.x + E); x <= (size_t)floor(pos.x + half.x - E); x++) {
			for (int z = floor(pos.z - half.z + E); z <= floor(pos.z + half.z - E); z++) {
				int y = floor(pos.y + half.y + E);
				if (chunks.isObstacle(x, y, z)) {
					vel.y = 0.f;
					pos.y = y - half.y - E;
					break;
				}
			}
		}
	}
}


glm::ivec3 off[] = {
		{-1,0,0 },
		{1,0,0 },
		{0,0,-1 },
		{0,0,1 },
		{0,1,0 },
		{0,-1,0 },
};
inline bool isSource(Voxel cur, Voxel tar) { return (cur.e.id == tar.e.id) && (cur.e.m1 < tar.e.m1); }
inline bool isSource(size_t cur, glm::ivec3 targ, Voxels& voxs) { return voxs.is(targ) == 0 ? 0 : isSource(voxs[cur], voxs(targ)); }
inline bool isFree(Voxel dest, Voxel source) {
	return !((dest.id_ == source.id_) && (dest.e.m1 >= source.e.m1 - 1));
}
void spreadLiquid(Voxel src, Voxels& voxs, Chunk* chunk,const glm::ivec3& local) {
	if (src.e.m1 > 0)
		for (size_t k = 0; k < 4; k++) {
			Voxel* dst = voxs.get(local + off[k]);
			if (dst && VoxPack::isGas(*dst))
				chunk->setVoxel(Voxel(src.id_, 0), local + off[k] + glm::ivec3(chunk->voxelPos()));
		}
	//if top
	Voxel* dst = voxs.get(local + off[5]);
	if (dst && VoxPack::isGas(*dst)) chunk->setVoxel(Voxel(src.id_, 0), local + off[5] + glm::ivec3(chunk->voxelPos()));
}

void PhysicsSolver::modelingLuqid(size_t i,Voxels& voxs, Chunk* chunk) {
	glm::ivec3 local = voxs.coord(i);
	//test source
	Voxel src = voxs[i];
	if (src.e.m1 == VoxPack::maxConcLiquid) {
		spreadLiquid(src, voxs, chunk, local);
		return;
	}
	byte m1 =0;
	for (size_t k = 0; k < 4; k++) {
		const Voxel* targ = voxs.get(local + off[k]);
		if(targ && isSource(voxs[i], *targ)&& targ->e.m1>m1) m1 = targ->e.m1;
	}
	const Voxel* targ = voxs.get(local + off[5]);
	if (targ && (targ->e.id == voxs[i].e.id)) m1 = VoxPack::maxConcLiquid - 1;
	if (m1 == 0) {
		//source is not exist
		//to air
		for (size_t k = 0; k < 6; k++) {
			glm::ivec3 pos(local + off[k]);
			if (voxs.is(pos)) chunk->nonStatic.insert(voxs.ind(pos));
		}
		if (src.e.m1 == 0) chunk->setVoxelLocal(vox::air, local);
		//else decrement m1
		else chunk->setVoxelLocal(Voxel(src.e.id, src.e.m1 - 1), local);
	}
	else {
		if(m1> src.e.m1+1) chunk->setVoxelLocal(Voxel(src.e.id, src.e.m1 +1), local);
		spreadLiquid(src, voxs, chunk, local);
	}
}

void PhysicsSolver::step_world() {
	
	ChunkPtrs& chunks = world->chunks.chunks();
	for (size_t j = 0; j < chunks.size(); j++) {
		Voxels& voxs = chunks[j]->voxels();
		std::set<size_t> buffer;
		chunks[j]->nonStatic.swap(buffer);
		
		for (size_t i : buffer) {
			if (VoxPack::isActive(chunks[j]->voxels()[i]) == 0)continue;
			if (VoxPack::isLiquid(voxs[i])) {
				modelingLuqid(i, voxs, chunks[j]);
			}
			else LOG(LogError, "Physics it isn't liquid\n");
		}
		if (chunks[j]->nonStatic.size())LOG("NonStatic:%d\n", chunks[j]->nonStatic.size());
	}
	
}
