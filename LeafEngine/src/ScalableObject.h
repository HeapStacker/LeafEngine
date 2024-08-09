#pragma once
#include "SpatialObject.h"
#include <vector>

namespace lf {
	class ScalableObject : public SpatialObject {
		bool traversed = false;
		std::vector<ScalableObject*> linkedToScale;
	public:
		glm::vec3 getScale();
		void scaleOnly(float scalar);
		void scaleOnly(const glm::vec3& scalar);
		void linkToScale(ScalableObject* object);
		void unlinkFromScale(ScalableObject* object);
	};
}