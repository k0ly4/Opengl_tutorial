#include "PhysicsSolver.h"

#define E 0.03f

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


glm::ivec3 dist[] = {
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
	for (size_t k = 0; k < 4; k++) {
		Voxel* dst = voxs.get(local + dist[k]);
		if ((dst == 0) || VoxPack::isSolid(*dst)) continue;
		if (src.e.id == dst->e.id) {
			if (src.e.m1 > dst->e.m1+1)
				chunk->setVoxel(Voxel(src.id_, dst->e.m1+1), local + dist[k] + glm::ivec3(chunk->voxelPos()));
		}
		else if(src.e.m1>0) chunk->setVoxel(Voxel(src.id_, 0), local + dist[k] + glm::ivec3(chunk->voxelPos()));
	}

	//if top
	Voxel* dst = voxs.get(local + dist[5]);
	if ((dst == 0) || VoxPack::isSolid(*dst)) return;
	if (src.e.id == dst->e.id) {
		if (VoxPack::maxConcLiquid > dst->e.m1+1) chunk->setVoxel(Voxel(src.id_, dst->e.m1 + 1), local + dist[5] + glm::ivec3(chunk->voxelPos()));
	}
	else if (src.e.m1 > 0) chunk->setVoxel(Voxel(src.id_, 0), local + dist[5] + glm::ivec3(chunk->voxelPos()));
}

void PhysicsSolver::modelingLuqid(size_t i,Voxels& voxs, Chunk* chunk) {
	glm::ivec3 local = voxs.coord(i);
	//test source
	Voxel src = voxs[i];
	if (src.e.m1 == VoxPack::maxConcLiquid) {
		spreadLiquid(src, voxs, chunk, local);
		return;
	}
	for (size_t k = 0; k < 5; k++) {
		if (isSource(i, local + dist[k], voxs)) {
			spreadLiquid(src, voxs, chunk, local);
			return;
		}
	}
	//source is not exist
	//to air
	if (src.e.m1 == 0) chunk->setVoxelLocal(vox::air, local);
	//else decrement m1
	else chunk->setVoxelLocal(Voxel(src.e.id, src.e.m1 - 1), local);
}

void PhysicsSolver::step_world() {
	
	ChunkPtrs& chunks = world->chunks.chunks();
	for (size_t j = 0; j < chunks.size(); j++) {
		Voxels& voxs = chunks[j]->voxels();
		ContainerNonStaticVoxel buffer = chunks[j]->nonStatic;
		for (size_t i : buffer.voxs_) {
			if (VoxPack::isLiquid(voxs[i])) {
				modelingLuqid(i,voxs,chunks[j]);
			}
			else LOG(LogError, "Physics it isn't liquid\n");
		}
	}
	
}
