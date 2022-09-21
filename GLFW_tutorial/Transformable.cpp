#include "Transformable.h"

	void Transformable3D::setupModel() {
		if (needUpMatrix) {
			model = glm::mat4(1.f);
			model = glm::translate(model, transform.position);
			model = glm::rotate(model, glm::radians(transform.rotate.angle), transform.rotate.axis);
			model = glm::scale(model, transform.scale);
			needUpMatrix = 0;
		}
	}
	Transformable3D::Transformable3D() : model(1.f) {
		transform.position = glm::vec3(0.f);
		transform.scale = glm::vec3(1.f);
		transform.rotate = Angle3D(0.f, glm::vec3(0.f, 1.f, 0.f));
	}