#pragma once
#include "PositionalLight.h"

namespace lf {
	class SpotLight : public PositionalLight {
		float ambientMultiplier, innerCutOff, outerCutOff;
		glm::vec3 color, direction;
	public:
		SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color = { 1.f, 1.f, 1.f }, float innerCutOff = 45.f, float outerCutOff = 60.f, Attenuation attenuation = getAttenuation(4), float ambientMultiplier = 1.f);
		void changeColor(glm::vec3 color);
		void changeInnerCutOff(float innerCutOff);
		void changeOuterCutOff(float outerCutOff);
		void changeAmbientMultiplier(float ambientMultiplier);
		static void UpdateSpotLights();
	};
}