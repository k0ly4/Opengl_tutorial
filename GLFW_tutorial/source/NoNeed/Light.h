#ifndef LIGHT3D_H
#define LIGHT3D_H

#include "Graphic/RenderWindow.h"

class Light
{
public:

	Light() :color_(0.f) {}

	Light(const glm::vec3& color) :color_(color) {}

	const glm::vec3& getColor()const {
		return color_;
	}

	inline bool isActive()const {
		return active_;
	}

	inline void setActive(bool enable) {
		active_ = enable;
	}

	virtual void setColor(const glm::vec3& color) {
		color_ = color;
	}

	inline virtual void uniform(const std::string& name,const Shader& shader)const {
		shader.uniform(name + ".color", color_);
	}

protected:
	bool active_ = 1;
	glm::vec3 color_;

private:
	
};
#endif

