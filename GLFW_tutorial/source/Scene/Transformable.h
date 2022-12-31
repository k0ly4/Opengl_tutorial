#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "Resource/Shader.h"
#include "Math/Transform.h"
//
///// <summary>
///// Transformable2D
///// </summary>
//class Transformable2D {
//
//public:
//	Transformable2D(const glm::vec2& position, const glm::vec2& scale, const glm::vec2& origin, float rotate) :
//		transform_(position, scale, origin, rotate) {}
//
//	Transformable2D() :
//		transform_(glm::vec2(0.f), glm::vec2(1.f), glm::vec2(0.f), 0.f) {}
//
//	inline virtual void setPosition(const glm::vec2& position) {
//		transform_.setPosition(position);
//	}
//
//	inline virtual void setScale(const glm::vec2& scale) {
//		transform_.setScale(scale);
//	}
//
//	inline virtual void setOrigin(const glm::vec2& origin) {
//		transform_.setOrigin(origin);
//	}
//
//	inline virtual void setRotate(float angle_in_radians) {
//		transform_.setRotate(angle_in_radians);
//	}
//
//	inline const glm::vec2& getPosition()const {
//		return transform_.getPosition();
//	}
//
//	inline const glm::vec2& getScale()const {
//		return transform_.getScale();
//	}
//
//	inline float getRotate()const {return transform_.getRotate();}
//
//	inline const glm::vec2& getOrigin()const {return transform_.getOrigin();}
//
//	inline void Move(glm::vec2 offset) {setPosition(getPosition() + offset);}
//
//	inline void Scale(glm::vec2 offset) {setScale(getScale() + offset);}
//
//	inline void Rotate(float offset) {setRotate(getRotate() + offset);}
//
//
//	void setPosition(float x, float y) {
//		setPosition(glm::vec2(x, y));
//	}
//
//	void setScale(float x, float y) {
//		setScale(glm::vec2(x, y));
//	}
//
//	void setOrigin(float x, float y) {
//		setOrigin(glm::vec2(x, y));
//	}
//
//	const Transform2D& getTransform()const {
//		return transform_;
//	}
//	void setTransform(const Transform2D& transform) {
//		transform_ = transform;
//	}
//
//protected:
//	
//
//	inline void uniformTransform(const Shader& shader)const {
//		shader.uniform("model", transform_.getModel());
//	}
//
//	Transform2D transform_;
//
//};

/// <summary>
/// Transformable3D
/// </summary>
class Transformable3D {

public:

	Transformable3D(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& origin, const AngleAxis& rotate) :
		transform_(position, scale, origin, rotate) 
	{}

	Transformable3D() :
		Transformable3D(
			glm::vec3(0.f),
			glm::vec3(1.f),
			glm::vec3(0.f),
			AngleAxis(glm::vec4(0.f,0.f,1.f,0.f))) 
	{}
	//Transform3D

	void setPosition(const glm::vec3& position) {
		transform_.setPosition(position);
	}
	const glm::vec3& getPosition()const {
		return transform_.getPosition();
	}

	void setScale(const glm::vec3& scale) {
		transform_.setScale(scale);
	}
	const glm::vec3& getScale()const {
		return transform_.getScale();
	}
	//In radians
	void setRotate(const AngleAxis& angle) {
		transform_.setRotate(angle);
	}
	//In radians
	const AngleAxis& getRotate()const {
		return transform_.getRotate();
	}

	void setOrigin(const glm::vec3& origin) {
		transform_.setOrigin(origin);
	}

	const glm::vec3& getOrigin()const {
		return transform_.getOrigin();
	}

	void setTransform(const Transform3D& transform) {
		transform_ = transform;
	}

	const Transform3D& getTransform()const {
		return transform_;
	}

	inline void Move(const glm::vec3& offset) {
		setPosition(transform_.getPosition() + offset);
	}
	inline void Scale(const glm::vec3& offset) {
		setScale(transform_.getScale() + offset);
	}
	inline void Rotate(float offset) {
		setRotate(AngleAxis(transform_.getRotate().angle + offset, transform_.getRotate().axis));
	}

protected:

	inline void uniformTransform(const Shader& shader) {
		shader.uniform("model", transform_.getModel());
	}

private:

	Transform3D transform_;

};
#endif
