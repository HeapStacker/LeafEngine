#pragma once
#include "ScalableObject.h"
#include "RotateableObject.h"
#include "MoveableObject.h"
#include "ShaderImplementation.h"

namespace lf {
	class TransformableObject : public ScalableObject, public RotateableObject, public MoveableObject {
		std::vector<TransformableObject*> linkedObjects;
	protected:
		glm::mat4 modelMatrix = glm::mat4(1.f);
	public:
		TransformableObject();
		void linkToRotation(TransformableObject* object);
		void rotateAround(glm::vec3 axis, float degrees);
	};
}