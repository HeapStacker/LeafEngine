#pragma once
#include "MovableObject.h"
#include "DiminishableLight.h"
#include "TransformableObject.h"

namespace lf {
	class PointLight : public DiminishableLight, public MovableObject {
		unsigned int id;
		std::vector<PointLight*> linkedToPosition;
		void update();
	public:
		PointLight(glm::vec3 position = {0.f, 0.f, 0.f}, glm::vec3 color = { 0.6f, 0.8f, 0.f }, unsigned int intensity = 1);
		PointLight(MovableObject* position, glm::vec3 color = { 0.6f, 0.8f, 0.f }, unsigned int intensity = 1);

		void setPosition(const glm::vec3& position) { MovableObject::setPosition(position); update(); }
		void translate(const glm::vec3& position) { MovableObject::translate(position); update(); }
		void linkTotranslation(TransformableObject* object) { MovableObject::linkToTranslation(object); update(); }
		void unlinkFromPosition(PointLight* object) { MovableObject::unlinkFromTranslation(object); update(); }

		void changeColor(const glm::vec3& color) { Light::changeColor(color); update(); }
		void changeAmbientMultiplier(float multiplier) { Light::changeAmbientMultiplier(multiplier); update(); }

		void changeIntensity(unsigned int intensity) { DiminishableLight::changeIntensity(intensity); update(); }
		void changeIntensity(const Attenuation& attenuation) { DiminishableLight::changeIntensity(attenuation); update(); }
	};
}