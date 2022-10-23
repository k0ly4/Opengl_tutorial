#include "Player.h"

void Player::upVoxelCursor(ChunkHandle& chunks) {

	glm::vec3 end;
	glm::vec3 norm;
	glm::ivec3 iend;

	if (chunks.rayCast(getBasis(), maxDistanceCursor, end, normCursor, posCursor) ==0 )
		posCursor = glm::ivec3(0);

	cube.setPosition(glm::vec3(posCursor) + glm::vec3(0.5f));
}

void Player::setVoxel(ChunkHandle& chunks, bool isModAdd) {
	if (posCursor == glm::ivec3(0))return;
	if (isModAdd) {
		chunks.setVoxel(Voxel(1), posCursor + glm::ivec3(normCursor));
	}
	else {
		chunks.setVoxel(Voxel(-1), posCursor );
	}
}