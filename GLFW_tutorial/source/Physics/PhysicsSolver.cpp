#include "PhysicsSolver.h"

#define E 0.03

void PhysicsSolver::test_step(Hitbox& hitbox) {
	hitbox.grounded = false;
	float dt = timeStep;
	//float linear_damping = hitbox.linear_damping;

	glm::vec3& pos = hitbox.position;
	glm::vec3& half = hitbox.halfsize;
	glm::vec3& vel = hitbox.velocity;
	float px = pos.x;
	float pz = pos.z;
	vel = vel + gravity * dt * hitbox.getGravityScale();
	ChunkSectorRender& chunks = world->chunks;
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
	//vel = vel + gravity * dt * hitbox.getGravityScale();

	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	pos.z += vel.z * dt;

}

void PhysicsSolver::step(Hitbox& hitbox) {
	hitbox.grounded = false;
	float dt = timeStep;
	//float linear_damping = hitbox.linear_damping;

	glm::vec3& pos = hitbox.position;
	glm::vec3& half = hitbox.halfsize;
	glm::vec3& vel = hitbox.velocity;

	vel = vel + gravity * dt * hitbox.getGravityScale();

	float px = pos.x;
	float pz = pos.z;
	ChunkSectorRender& chunks = world->chunks;
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

	/*vel.x *= max(0.0, 1.0 - dt * linear_damping);
	vel.z *= max(0.0, 1.0 - dt * linear_damping);*/

	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	pos.z += vel.z * dt;

	/*if (shifting && hitbox->grounded) {
		int y = floor(pos.y - half.y - E);

		hitbox->grounded = false;
		for (int x = floor(px - half.x + E); x <= floor(px + half.x - E); x++) {
			for (int z = floor(pos.z - half.z + E); z <= floor(pos.z + half.z - E); z++) {
				if (chunks->isObstacle(x, y, z)) {
					hitbox->grounded = true;
					break;
				}
			}
		}
		if (!hitbox->grounded)
			pos.z = pz;
		hitbox->grounded = false;

		for (int x = floor(pos.x - half.x + E); x <= floor(pos.x + half.x - E); x++) {
			for (int z = floor(pz - half.z + E); z <= floor(pz + half.z - E); z++) {
				if (chunks->isObstacle(x, y, z)) {
					hitbox->grounded = true;
					break;
				}
			}
		}
		if (!hitbox->grounded)
			pos.x = px;

		hitbox->grounded = true;
	}*/
	
}