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
		for (size_t y = floor(pos.y - half.y + E); y <= floor(pos.y + half.y - E); y++) {
			for (size_t z = floor(pos.z - half.z + E); z <= floor(pos.z + half.z - E); z++) {
				size_t x = floor(pos.x - half.x - E);

				if (chunks.isObstacle(glm::uvec3(x, y, z))) {
					vel.x = 0.f;
					pos.x = x + 1 + half.x + E;
					break;
				}
			}
		}
	}
	if (vel.x > 0.0) {
		for (size_t y = floor(pos.y - half.y + E); y <= floor(pos.y + half.y - E); y++) {
			for (size_t z = floor(pos.z - half.z + E); z <= floor(pos.z + half.z - E); z++) {
				size_t x = floor(pos.x + half.x + E);
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
		for (size_t y = floor(pos.y - half.y + E); y <= floor(pos.y + half.y - E); y++) {
			for (size_t x = floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
				size_t z = floor(pos.z - half.z - E);
				if (chunks.isObstacle(x, y, z)) {
					vel.z = 0.f;
					pos.z = z + 1 + half.z + E;
					break;
				}
			}
		}
	}

	if (vel.z > 0.0) {
		for (size_t y = floor(pos.y - half.y + E); y <= floor(pos.y + half.y - E); y++) {
			for (size_t x = floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
				size_t z = floor(pos.z + half.z + E);
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
		for (size_t x = floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
			for (size_t z = floor(pos.z - half.z + E); z <= floor(pos.z + half.z - E); z++) {
				size_t y = floor(pos.y - half.y - E);
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
		for (int x = floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
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
		{0,-1,0 },
		{0,0,-1 },
		{0,0,1 },
};
void parse(const glm::ivec3& coord, Voxels& voxs, Chunk* chunk,Voxel targ) {
	Voxel* v = voxs.get(coord);
	if (v == 0|| VoxPack::isSolid(*v))return;
	//Если здесь уже есть такая же жидкость большей концентрации
	if (targ.e.id == v->e.id && targ.e.m1 <= v->e.m1) return;
	chunk->setVoxel(targ, coord+ glm::ivec3(chunk->voxelPos()));
}

void PhysicsSolver::modelingLuqid(size_t i,Voxels& voxs, Chunk* chunk) {
	glm::ivec3 coord = voxs.coord(i);
	Voxel targ = Voxel(voxs[i].e.id, voxs[i].e.m1 == 0 ? 0 : voxs[i].e.m1 - 1);
	for (size_t k = 0; k < 5; k++) 
		parse(coord + off[k], voxs, chunk, targ);
}

void PhysicsSolver::step_world() {
	
	ChunkPtrs& chunks = world->chunks.chunks();
	for (size_t j = 0; j < chunks.size(); j++) {
		Voxels& voxs = chunks[j]->voxels();
		for (size_t i : chunks[j]->nonStatic.voxs_) {
			if (VoxPack::isLiquid(voxs[i])) {
				modelingLuqid(i,voxs,chunks[j]);
			}
			else LOG(LogError, "Physics it isn't liquid\n");
		}
	}
	
}
