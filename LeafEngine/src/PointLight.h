#pragma once
#include "PositionalLight.h"

namespace lf {
	class PointLight : public PositionalLight {
		float ambientMultiplier;
		glm::vec3 color;
	public:
		static void UpdatePointLights();
		PointLight(glm::vec3 position = {0.f, 0.f, 0.f}, glm::vec3 color = {1.f, 1.f, 1.f}, Attenuation attenuation = getAttenuation(4), float ambientMultiplier = 1.f);
		void changeColor(glm::vec3 color);
		void changeAmbientMultiplier(float ambientMultiplier);
	};
}