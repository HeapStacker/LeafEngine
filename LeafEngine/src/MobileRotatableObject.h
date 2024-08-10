#pragma once
#include "RotateableObject.h"
#include "MovableObject.h"

namespace lf {
	class MobileRotatableObject : public RotateableObject, public virtual MovableObject {
		bool traversed = false;
		std::vector<MobileRotatableObject*> linkedToRotationTranslation;
	public:
		void rotateTranslate(glm::vec3 axis, float degrees);
		void linkToRotationTranslation(MobileRotatableObject* object);
		void unlinkFromRotationTranslation(MobileRotatableObject* object);
	};
}
