#pragma once
#include <glm/glm.hpp>

namespace lf {
	struct Light {
	protected:
		glm::vec3 color = { 0.6f, 0.8f, 0.f };
		float ambientMultiplier = 1.f;
	public:
		void changeColor(const glm::vec3& color) { this->color = color; }
		void changeAmbientMultiplier(float multiplier) { this->ambientMultiplier = multiplier; }
	};
}