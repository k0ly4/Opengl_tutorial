#ifndef LIGHT3D_H
#define LIGHT3D_H

#include "RenderWindow.h"

class Light
{
public:

	Light() :color_(0.f) {}

	Light(const glm::vec3& color) :color_(color) {}

	const glm::vec3& getColor()const {
		return color_;
	}

	virtual void setColor(const glm::vec3& color) {
		color_ = color;
	}

	inline virtual void uniform(const std::string& name,const Shader& shader)const {
		shader.uniform(name + ".color", color_);
	}

protected:

	glm::vec3 color_;

private:
	
};
#endif

