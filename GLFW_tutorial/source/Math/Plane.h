#ifndef MATH_PLANE_H
#define MATH_PLANE_H

#include "GLM.h"

class Plane
{

public:

	glm::vec3 normal, point;
	float d;


	Plane(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
		set(v1, v2, v3);
	}

	Plane() {}
	~Plane() {}

	inline void set(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
		point = v2;
		normal = glm::normalize(glm::cross( (v3 - v2), (v1 - v2)));
		d = -(glm::dot(this->normal, point));
	}
	inline void set(const glm::vec3& normal_, const glm::vec3& point_) {
		this->normal = normal_;
		d = -(glm::dot(this->normal, point_));
	}
	inline void set(float a, float b, float c, float d) {
		//compute the lenght of the vector
		float l = glm::length(glm::vec3(a, b, c));
		// normalize the vector
		normal = { a / l, b / l, c / l };
		// and divide d by th length as well
		this->d = d / l;
	}
	inline float distance(const glm::vec3& p) { return (d + glm::dot(normal, p)); };

};
class AABox
{

public:

	glm::vec3 corner;
	glm::vec3 s;


	AABox(const glm::vec3& corner, float x, float y, float z) {
		set(corner, x, y, z);
	}
	AABox(const glm::vec3& corner, const glm::vec3& s_) {
		set(corner, s_.x,s_.y,s_.z);
	}
	AABox() :corner(0.f), s(1.f) {}

	~AABox() {}

	inline void set(const glm::vec3& corner_, float x_, float y_, float z_) {
		corner = corner_;

		if (x_ < 0.0) {
			x_ = -x_;
			this->corner.x -= x_;
		}
		if (y_ < 0.0) {
			y_ = -y_;
			this->corner.y -= y_;
		}
		if (z_ < 0.0) {
			z_ = -z_;
			this->corner.z -= z_;
		}
		s = { x_,y_,z_ };
	}


	// for use in frustum computations
	inline glm::vec3 getVertexP(const glm::vec3& normal) const {
		glm::vec3 res(corner);

		if (normal.x > 0)res.x += s.x;
		if (normal.y > 0)res.y += s.y;
		if (normal.z > 0)res.z += s.z;

		return res;
	}
	inline glm::vec3 getVertexN(const glm::vec3& normal) const {

		glm::vec3 res(corner);

		if (normal.x < 0)	res.x += s.x;
		if (normal.y < 0) res.y += s.y;
		if (normal.z < 0) res.z+= s.z;

		return res;
	}
};
#endif