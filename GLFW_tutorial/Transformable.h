#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "Shader.h"
#include "Transform.h"

/// <summary>
/// Transformable2D
/// </summary>
class Transformable2D {

public:
	Transformable2D(const glm::vec2& position, const glm::vec2& scale, const glm::vec2& origin, float rotate) :
		transform_(position, scale, origin, rotate) {}

	Transformable2D() :
		transform_(glm::vec2(0.f), glm::vec2(1.f), glm::vec2(0.f), 0.f) {}

	inline virtual void setPosition(const glm::vec2& position) {
		transform_.setPosition(position);
	}

	inline virtual void setScale(const glm::vec2& scale) {
		transform_.setScale(scale);
	}

	inline virtual void setOrigin(const glm::vec2& origin) {
		transform_.setOrigin(origin);
	}

	inline virtual void setRotate(float angle_in_radians) {
		transform_.setRotate(angle_in_radians);
	}

	inline const glm::vec2& getPosition()const {
		return transform_.getPosition();
	}

	inline const glm::vec2& getScale()const {
		return transform_.getScale();
	}

	inline float getRotate()const {
		return transform_.getRotate();
	}

	inline const glm::vec2& getOrigin()const {
		return transform_.getOrigin();
	}

	inline void Move(glm::vec2 offset) {
		setPosition(getPosition() + offset);
	}

	inline void Scale(glm::vec2 offset) {
		setScale(getScale() + offset);
	}

	inline void Rotate(float offset) {
		setRotate(getRotate() + offset);
	}


	void setPosition(float x, float y) {
		setPosition(glm::vec2(x, y));
	}

	void setScale(float x, float y) {
		setScale(glm::vec2(x, y));
	}

	void setOrigin(float x, float y) {
		setOrigin(glm::vec2(x, y));
	}

	const Transform2D& getTransform()const {
		return transform_;
	}
	void setTransform(const Transform2D& transform) {
		transform_ = transform;
	}

protected:

	inline void uniformTransform(const Shader& shader)const {
		shader.uniform("model", transform_.getModel());
	}

	Transform2D transform_;

};

/// <summary>
/// Transformable3D
/// </summary>
class Transformable3D {

public:
	Transformable3D();
	//Transform3D
	void setTransform(const Transform3D& _transform) {
		transform = _transform;
		needUpMatrix = 1;
	}
	const Transform3D& getTransform()const {
		return transform;
	}
	//Position
	void setPosition(const glm::vec3& Position) {
		transform.position = Position;
		needUpMatrix = 1;
	}
	const glm::vec3& getPosition()const {
		return transform.position;
	}
	inline void Move(const glm::vec3& offset) {
		setPosition(transform.position + offset);
	}
	//Scale
	void setScale(const glm::vec3& Scale) {
		transform.scale = Scale;
		needUpMatrix = 1;
	}
	const  glm::vec3& getScale()const {
		return transform.scale;
	}
	inline void Scale(const glm::vec3& offset) {
		setScale(transform.scale + offset);
	}
	//Rotate
	void setRotate(const Angle3D& Angle) {
		transform.rotate = Angle;
		needUpMatrix = 1;
	}
	const  Angle3D& getRotate()const {
		return transform.rotate;
	}
	inline void Rotate(float offset) {
		setRotate(Angle3D(transform.rotate.angle + offset, transform.rotate.axis));
	}

protected:
	inline void uniformTransform(const Shader& shader) {
		shader.uniform("model", getModel());
	}
	const inline glm::mat4& getModel() {
		setupModel();
		return model;
	}

private:
	bool needUpMatrix = 1;
	glm::mat4 model;
	Transform3D transform;

	void setupModel();
};
#endif
