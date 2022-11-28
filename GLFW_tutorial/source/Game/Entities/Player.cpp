#include "Player.h"
#include "Input/Keyboard.h"


void InputPlayer::moveUpdate(float time) {
	const float speed = 10.f;
	glm::vec3 vec_move(0.f);

	hitbox->velocity.x = hitbox->velocity.y= hitbox->velocity.z =0.f;

	if (Keyboard::getKey(Keyboard::W).action) {
		vec_move.z = 1;
		hitbox->velocity += maxSpeed * camera->getBasis().front;
	}
	else if (Keyboard::getKey(Keyboard::S).action) {
		vec_move.z = -1;
		hitbox->velocity += -maxSpeed * camera->getBasis().front;
	}
	if (Keyboard::getKey(Keyboard::D).action) {
		vec_move.x = 1;
		hitbox->velocity += maxSpeed * glm::normalize(glm::cross(camera->getBasis().front, GAME::WORLD_UP));
	}
	else if (Keyboard::getKey(Keyboard::A).action) {
		vec_move.x = -1;
		hitbox->velocity += -maxSpeed * glm::normalize(glm::cross(camera->getBasis().front, GAME::WORLD_UP));
	}
	////  camera.move(glm::vec3(vec_move.x * time * speed, 0.f, vec_move.z * time * speed));
	//float s = sqrt(abs(vec_move.z) + abs(vec_move.x));
	//if (s > 0.f) {
	//	s = (speed * time) / s;
		//camera->move(glm::vec3(vec_move.x * s, 0.f, vec_move.z * s));
	//}

}

void InputPlayer::setVoxel(World& world, bool isModAdd) {
	if (posCursor == glm::ivec3(0))return;

	if (isModAdd) {
		glm::ivec3 pos(posCursor + glm::ivec3(normCursor));
		world.chunks.setVoxel(curVoxel, pos);
		world.light.add(pos, curVoxel.id);
	}
	else {
		world.chunks.setVoxel(Voxel(-1), posCursor);
		world.light.remove(posCursor);
	}
}

void Cursor3D::update(ChunkSectorRender& chunks, InputPlayer& input, const Basis& basis) {

	glm::vec3 end;

	if (chunks.rayCast(basis, maxDistanceCursor, end, input.normCursor, input.posCursor) ==0 )
		input.posCursor = glm::ivec3(0);

	cube.setPosition(glm::vec3(input.posCursor) + glm::vec3(0.5f));
}

