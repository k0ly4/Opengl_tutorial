#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "Shader.h"
#include "Transform.h"
class Transformable2D {
protected:
	float rotate;
	glm::vec2 scale;
	glm::vec2 position;
	glm::vec2 origin;
public:
	virtual void setPosition(glm::vec2) = 0;
	virtual void setScale(glm::vec2) = 0;
	virtual void setOrigin(glm::vec2) = 0;
	virtual void setRotate(float) = 0;
	Transformable2D() :scale(1.f), position(0.f), origin(0.f), rotate(0.f) {}

	inline void Move(glm::vec2 offset) {
		setPosition(position + offset);
	}
	inline void Scale(glm::vec2 offset) {
		setScale(scale + offset);
	}
	inline void Rotate(float offset) {
		setRotate(rotate + offset);
	}
};
class Transformable:public Transformable2D {
protected:
	bool need_update_model = 0;
public:
	inline void setPosition(glm::vec2 position) {
		this->position = position;
		need_update_model = 1;
	}
	inline void setRotate(float angle) {
		rotate = angle;
		need_update_model = 1;
	}
	inline void setOrigin(glm::vec2 origin) {
		this->origin = origin;
		need_update_model = 1;
	}
	inline void setScale(glm::vec2 scale) {
		this->scale = scale;
		need_update_model = 1;
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

};
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
