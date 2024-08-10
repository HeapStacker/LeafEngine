#pragma once
#include "SpatialObject.h"
#include <vector>

namespace lf {
	class ScalableObject : public virtual SpatialObject {
		bool traversed = false;
		std::vector<ScalableObject*> linkedToScale;
	public:
		glm::vec3 getScale();
		void scale(float scalar);
		void scale(const glm::vec3& scalar);
		void linkToScale(ScalableObject* object);
		void unlinkFromScale(ScalableObject* object);
	};
}