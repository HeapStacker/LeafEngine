#pragma once
#include "PositionalLight.h"
#include <glm/glm.hpp>

namespace lf {
	class DirectionalLight {

		/*Default values*/
		glm::vec3 direction = {1.f, -1.f, -1.f};
		glm::vec3 color = { 1.f, 1.f, 1.f };
		float ambientMultiplier = 1.f;
		static void UpdateDirectionalLights();
	public:
		DirectionalLight(glm::vec3 color = { 1.f, 1.f, 1.f }, glm::vec3 direction = { -1.f, -1.f, 1.f });
		void ChangeDirection(glm::vec3 direction) { this->direction = direction; UpdateDirectionalLights(); }
		void ChangeColor(glm::vec3 color) { this->color = color; UpdateDirectionalLights(); }
	};
}