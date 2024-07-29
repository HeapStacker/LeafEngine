#include <vector>
#include "RotateableObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace lf {
	void RotateableObject::rotateAround(glm::vec3 axis, float degrees)
	{
		*linkedModelMatrixR = glm::rotate(*linkedModelMatrixR, glm::radians(degrees), glm::normalize(axis));
	}
}
