#include "ScalableObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace lf {
	void ScalableObject::scale(float scalar)
	{
		*linkedModelMatrixS = glm::scale(*linkedModelMatrixS, glm::vec3(scalar));
	}

	void ScalableObject::scale(const glm::vec3& scalar)
	{
		*linkedModelMatrixS = glm::scale(*linkedModelMatrixS, scalar);
	}
}
