#pragma once
#include "DiminishableLight.h"
#include "MobileRotatableObject.h"

namespace lf {
	class SpotLight : public MobileRotatableObject, public DiminishableLight {
		unsigned int id;
		float innerCutOff, outerCutOff;
		void update();
	public:
		SpotLight(glm::vec3 position = {0.f, 0.f, 0.f}, glm::vec3 orientation = {-1.f, -1.f, 1.f}, glm::vec3 color = {0.6f, 0.8f, 0.f}, float innerCutOff = 45.f, float outerCutOff = 60.f, unsigned int intensity = 1);
		SpotLight(MovableObject* position, glm::vec3 orientation = { -1.f, -1.f, 1.f }, glm::vec3 color = { 0.6f, 0.8f, 0.f }, float innerCutOff = 45.f, float outerCutOff = 60.f, unsigned int intensity = 1);

		void setPosition(const glm::vec3& position) { MobileRotatableObject::setPosition(position); update(); }
		void setPosition(MovableObject* position) { MobileRotatableObject::setPosition(position); update(); }
		void translate(const glm::vec3& position) { MobileRotatableObject::translate(position); update(); }

		void changeColor(const glm::vec3& color) { Light::changeColor(color); update(); }
		void changeAmbientMultiplier(float multiplier) { Light::changeAmbientMultiplier(multiplier); update(); }

		void changeIntensity(unsigned int intensity) { DiminishableLight::changeIntensity(intensity); update(); }
		void changeIntensity(const Attenuation& attenuation) { DiminishableLight::changeIntensity(attenuation); update(); }

		void changeInnerCutOff(float innerCutOff) { this->innerCutOff = innerCutOff; update(); }
		void changeOuterCutOff(float outerCutOff) { this->outerCutOff = outerCutOff; update(); }
	};
}