#include "Player.h"
#include "Input/Keyboard.h"


void InputPlayer::moveUpdate(float time) {
	hitbox->velocity.x = hitbox->velocity.z =0.f;
	const glm::vec3& vecRight = camera->getBasis().right;
	if (Keyboard::getKey(Keyboard::W).action) {
		hitbox->velocity += maxSpeed * glm::cross(GAME::WORLD_UP, vecRight);
	}
	else if (Keyboard::getKey(Keyboard::S).action) {
		hitbox->velocity += -maxSpeed * glm::cross(GAME::WORLD_UP,vecRight);
	}
	if (Keyboard::getKey(Keyboard::D).action) {
		hitbox->velocity += maxSpeed * vecRight;
	}
	else if (Keyboard::getKey(Keyboard::A).action) {
		hitbox->velocity += -maxSpeed * vecRight;
	}
}

void InputPlayer::setVoxel(World& world, bool isModAdd) {
	if (posCursor == glm::ivec3(0))return;

	if (isModAdd) {
		glm::ivec3 pos(posCursor + glm::ivec3(normCursor));
		world.chunks.setVoxel(curVoxel, pos);
		world.light.add(pos, curVoxel);
	}
	else {
		world.chunks.setVoxel(vox::air, posCursor);
		world.light.remove(posCursor);
	}
}

void Cursor3D::update(ChunkSectorRender& chunks, InputPlayer& input, const Basis& basis) {

	glm::vec3 end;

	if (chunks.rayCast(basis, maxDistanceCursor, end, input.normCursor, input.posCursor) ==0 )
		input.posCursor = glm::ivec3(0);

	cube.setPosition(glm::vec3(input.posCursor) + glm::vec3(0.5f));
}

