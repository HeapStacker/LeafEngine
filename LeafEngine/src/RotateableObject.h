#pragma once
#include <glm/glm.hpp>

namespace lf {
	class RotateableObject {
		//std::vector<RotateableObject*> linkedObjects;
	protected:
		glm::mat4* linkedModelMatrixR = nullptr;
		glm::mat4 rotationalOutcome = glm::mat4(1.f);
	public:
		void rotateAround(glm::vec3 axis, float degrees);
	};
}