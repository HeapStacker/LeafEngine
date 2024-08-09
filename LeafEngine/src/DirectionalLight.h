#pragma once
#include "Light.h"
#include "RotateableObject.h"

namespace lf {
	class DirectionalLight : public Light, public RotateableObject {
		unsigned int id;
		void update();
	public:
		DirectionalLight(const glm::vec3& color = { 0.6f, 0.8f, 0.f }, const glm::vec3& orientation = { -1.f, -1.f, 1.f });
		void changeColor(const glm::vec3& color) { Light::changeColor(color); update(); }
		void changeOrientation(const glm::vec3& orientation) { RotateableObject::orient(orientation); update(); }
	};
}