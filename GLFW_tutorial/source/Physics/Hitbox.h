#ifndef HITBOX_H
#define HITBOX_H
#include "Math/Math.h"

class Hitbox
{
public:

	glm::vec3 position;
	glm::vec3 halfsize;
	glm::vec3 velocity;
	float mass;
	//float linear_damping;
	bool grounded = 0;

	inline void create(const glm::vec3& position_,const glm::vec3& halfSize_,float mass_,const glm::vec3& startVelocity_ = glm::vec3(0.f),bool grounded_ =0) {
		position = position_;
		halfsize = halfSize_;
		mass = mass_;
		velocity = startVelocity_;
		grounded = grounded_;
	}
	Hitbox(const glm::vec3& position, const glm::vec3& halfsize);
	Hitbox(){}
};
#endif


