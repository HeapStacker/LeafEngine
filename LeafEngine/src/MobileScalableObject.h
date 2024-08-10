#include "ScalableObject.h"
#include "MovableObject.h"

namespace lf {
	class MobileScalableObject : public ScalableObject, public virtual MovableObject {
		bool traversed = false;
		std::vector<MobileScalableObject*> linkedToScaleTranslation;
	public:
		void scaleTranslate(float scalar);
		void scaleTranslate(const glm::vec3& scalar);
		void linkToScaleTranslation(MobileScalableObject* object);
		void unlinkFromScaleTranslation(MobileScalableObject* object);
	};
}