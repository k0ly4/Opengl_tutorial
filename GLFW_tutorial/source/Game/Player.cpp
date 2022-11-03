#include "Player.h"

void Player::upVoxelCursor(ChunkHandle& chunks) {

	glm::vec3 end;
	glm::vec3 norm;
	glm::ivec3 iend;

	if (chunks.rayCast(getBasis(), maxDistanceCursor, end, normCursor, posCursor) ==0 )
		posCursor = glm::ivec3(0);

	cube.setPosition(glm::vec3(posCursor) + glm::vec3(0.5f));
}

void Player::setVoxel(World& world, bool isModAdd) {
	LOG("R=%d,G=%d,B=%d,S=%d\n",
		world.chunks.getChannelLight(glm::ivec3(camera_->getPosition()), 0),
		world.chunks.getChannelLight(glm::ivec3(camera_->getPosition()), 1),
		world.chunks.getChannelLight(glm::ivec3(camera_->getPosition()), 2),
		world.chunks.getChannelLight(glm::ivec3(camera_->getPosition()), 3));
	if (posCursor == glm::ivec3(0))return;

	if (isModAdd) {
		glm::ivec3 pos(posCursor + glm::ivec3(normCursor));
		world.chunks.setVoxel(curVoxel, pos);
		world.light.add(pos, curVoxel.id);
	}
	else {
		world.chunks.setVoxel(Voxel(-1), posCursor );
		world.light.remove(posCursor);
	}
}