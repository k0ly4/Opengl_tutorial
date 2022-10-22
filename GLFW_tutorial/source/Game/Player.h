#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"

class Player
{
public:
	const glm::vec3& getPosition()const {
		return camera_->getPosition();
	}
	
	void setCamera(Camera& camera) {
		camera_ = &camera;
	}

private:
	
	Camera* camera_;
};

#endif