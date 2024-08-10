#pragma once
#include "SpatialObject.h"
#include <vector>

namespace lf {
	class MovableObject : public virtual SpatialObject {
		bool traversed = false;
		std::vector<MovableObject*> linkedToTranslation;
	public:
		glm::vec3&& getPosition();
		void setPosition(const glm::vec3& position);
		void setPosition(MovableObject* object);
		void translate(const glm::vec3& position);
		void linkToTranslation(MovableObject* object);
		void unlinkFromTranslation(MovableObject* object);
	};
}