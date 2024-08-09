#pragma once
#include <vector>
#include "glm/glm.hpp"

namespace lf {
	struct SpatialObject {
		glm::mat4 modelMatrix = glm::mat4(1.f);
	};
}