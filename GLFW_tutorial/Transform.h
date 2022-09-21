#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Game.h"

struct Angle3D {
	glm::vec3 axis;
	float angle;

	Angle3D() 
		:angle(0.f), axis(0.f) {}

	Angle3D(float Angle, const glm::vec3& Axis) 
		:angle(Angle), axis(Axis) {}

	Angle3D(const glm::vec4& rotate) 
		:axis(rotate.x, rotate.y, rotate.z), angle(rotate.w) {}
};

struct Transform3D {

	Angle3D rotate;
	glm::vec3 position;
	glm::vec3 scale;

	Transform3D() 
		:position(0.f), scale(1.f), rotate(0.f, glm::vec3(0.f, 1.f, 0.f)) {}

	Transform3D(const glm::vec3& Pos, const glm::vec3& Scale, const Angle3D& Rotate) 
		:rotate(Rotate), position(Pos), scale(Scale) {}

	glm::mat4 getModel() {
		return glm::scale(glm::rotate(glm::translate(glm::mat4(1.f), position), glm::radians(rotate.angle), rotate.axis), scale);
	}
};

struct Basis {

	glm::vec3
		right,
		up,
		front;
	glm::vec3 position;

	inline glm::mat4 lookAt()const {
		return glm::lookAt(position, position + front, GAME::WORLD_UP);
	}
};

struct AngleEuler {
	float yaw;
	float pitch;

	AngleEuler() :yaw(0.f), pitch(0.f) {}
	AngleEuler(float yaw_, float pitch_) :yaw(yaw_), pitch(pitch_) {}

	inline void normalizePitch(float angle) {
		if (pitch > angle) pitch = angle;
		else if (pitch < -angle)pitch = -angle;
	}

	void getBasis(Basis& basis) {
		basis.front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		basis.front.y = sin(glm::radians(pitch));
		basis.front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	}

};
#endif

