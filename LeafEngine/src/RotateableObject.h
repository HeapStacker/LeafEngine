#pragma once
#include "SpatialObject.h"
#include <vector>

namespace lf {
	class RotateableObject : public SpatialObject {
		bool traversed = false;
		std::vector<RotateableObject*> linkedToRotation;
	public:
		glm::vec3&& getRotation();
		std::vector<glm::vec3> getAxes();
		void orient(const glm::vec3& orientation);
		void rotateOnly(glm::vec3 axis, float degrees);
		void linkToRotation(RotateableObject* object);
		void unlinkFromRotation(RotateableObject* object);
	};
}
