#pragma once
#include <glm/glm.hpp>

namespace lf {
	class ScalableObject
	{
	protected:
		glm::mat4* linkedModelMatrixS = nullptr;
	public:
		void scale(float scalar);
		void scale(const glm::vec3& scalar);
	};
}

