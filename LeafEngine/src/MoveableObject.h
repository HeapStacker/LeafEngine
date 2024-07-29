#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace lf {
	class MoveableObject {
		std::vector<MoveableObject*> linkedObjects;
	protected:
		glm::mat4* linkedModelMatrixM = nullptr;
		glm::vec3 position = glm::vec3(0.f);
		bool isPointLight = false;
		bool isSpotLight = false;
	public:
		void setPosition(const glm::vec3& position);
		glm::vec3 getPosition();
		void translate(const glm::vec3& position);
		void linkToPosition(MoveableObject* object);
		void unlinkFromPosition(MoveableObject* object);
	};
}