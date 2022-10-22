#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Game/Game.h"

///AngleAxis----------------------------------------------------------
/// <summary>
///
/// </summary>
struct AngleAxis {

	glm::vec3 axis;
	float angle;

	AngleAxis()
		:angle(0.f), axis(0.f) {}

	AngleAxis(float Angle, const glm::vec3& Axis)
		:angle(Angle), 
		axis(Axis) {}

	AngleAxis(const glm::vec4& rotate)
		:axis(rotate.x, rotate.y, rotate.z), 
		angle(rotate.w) {}
};

///Transform3D----------------------------------------------------------
/// <summary>
/// 
/// </summary>
class Transform2D {

public:
	
	Transform2D(const glm::vec2& position, const glm::vec2& scale,const glm::vec2& origin, float rotate):
		rotate_(rotate), 
		position_(position), 
		scale_(scale),
		origin_(origin),
		model(1.f)
	{}

	void setPosition(const glm::vec2& position) {
		position_ = position;
		needUpModel = 1;
	}
	const glm::vec2& getPosition()const {
		return position_;
	}

	void setScale(const glm::vec2& scale) {
		scale_ = scale;
		needUpModel = 1;
	}
	const glm::vec2& getScale()const {
		return scale_;
	}
	//In radians
	void setRotate(float rotate_in_radians) {
		rotate_ = rotate_in_radians;
		needUpModel = 1;
	}
	//In radians
	float getRotate()const {
		return rotate_;
	}

	void setOrigin(const glm::vec2& origin) {
		origin_ = origin;
		needUpModel = 1;
	}
	const glm::vec2& getOrigin()const {
		return origin_;
	}

	const glm::mat4& getModel() const{
		if (needUpModel)calcModel();
		return model;
	}

	bool isNeedUp()const {
		return needUpModel;
	}
protected:

	void calcModel()const {
		model = glm::mat4(1.0f);
		//Rotate
		model = glm::translate(model, glm::vec3(origin_ + position_, 0.0f));
		model = glm::rotate(model, rotate_, glm::vec3(0.0f, 0.0f, 1.0f));
		//Move
		model = glm::translate(model, glm::vec3(-scale_ * origin_, 0.0f));
		//Scale
		model = glm::scale(model, glm::vec3(scale_, 1.0f)); // последним идет масштабирование

		needUpModel = 0;
	}

	mutable bool needUpModel = 1;
	mutable glm::mat4 model;

	float rotate_;
	glm::vec2 position_;
	glm::vec2 scale_;
	glm::vec2 origin_;
};

///Transform3D----------------------------------------------------------
/// <summary>
/// Transform3D
/// </summary>
class Transform3D {

public:

	Transform3D(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& origin, const AngleAxis& rotate) :
		rotate_(rotate),
		position_(position),
		scale_(scale),
		origin_(origin),
		model(1.f)
	{}

	void setPosition(const glm::vec3& position) {
		position_ = position;
		needUpModel = 1;
	}
	const glm::vec3& getPosition()const {
		return position_;
	}

	void setScale(const glm::vec3& scale) {
		scale_ = scale;
		needUpModel = 1;
	}
	const glm::vec3& getScale()const {
		return scale_;
	}
	//In radians
	void setRotate(const AngleAxis& angle) {
		rotate_ = angle;
		needUpModel = 1;
	}
	//In radians
	const AngleAxis& getRotate()const {
		return rotate_;
	}

	void setOrigin(const glm::vec3& origin) {
		origin_ = origin;
		needUpModel = 1;
	}
	const glm::vec3& getOrigin()const {
		return origin_;
	}

	const glm::mat4& getModel() const {
		if (needUpModel)calcModel();
		return model;
	}

	bool isNeedUp()const {
		return needUpModel;
	}

protected:

	void calcModel()const {
		model = glm::mat4(1.f);
		model = glm::translate(model, position_);
		model = glm::rotate(model, glm::radians(rotate_.angle), rotate_.axis);
		model = glm::scale(model, scale_);

		needUpModel = 0;
	}

	mutable bool needUpModel = 1;
	mutable glm::mat4 model;

	AngleAxis rotate_;
	glm::vec3 position_;
	glm::vec3 scale_;
	glm::vec3 origin_;
};

///Basis----------------------------------------------------------
/// <summary>
/// 
/// </summary>
struct Basis {

	glm::vec3
		right,
		up,
		front;

	glm::vec3 position;

	Basis(const glm::vec3& position_, const glm::vec3& right_, const glm::vec3& up_, const glm::vec3& front_) :
	position(position_),
	right(right_),
	up(up_),
	front(front_)
	{}

	Basis() :
		right(glm::vec3(1.f, 0.f, 0.f)),
		up(GAME::WORLD_UP),
		front(glm::vec3(0.f, 0.f, -1.f)),
		position(glm::vec3(0.f)) 
	{}

	inline glm::mat4 getMatrix()const {
		return glm::lookAt(position, position + front, GAME::WORLD_UP);
	}
};

inline bool operator ==(const Basis& left, const Basis& right) {
	return ((left.right == right.right) &&
			(left.up == right.up) &&
			(left.front == right.front) &&
			(left.position == right.position)
		);
}
inline bool operator !=(const Basis& left, const Basis& right) {
	return (!(left == right));
}

///AngleEuler----------------------------------------------------------
/// <summary>
/// AngleEuler
/// </summary>
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

