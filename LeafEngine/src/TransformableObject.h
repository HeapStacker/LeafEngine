#pragma once
#include "ScalableObject.h"
#include "MobileScalableObject.h"
#include "MobileRotatableObject.h"

namespace lf {
	class TransformableObject : public MobileRotatableObject, public MobileScalableObject {
	public:
		void linkTo(TransformableObject* transformableObject) { linkToTranslation(transformableObject); linkToRotation(transformableObject); linkToScale(transformableObject); linkToRotationTranslation(transformableObject); linkToScaleTranslation(transformableObject); }
		void unlinkFrom(TransformableObject* transformableObject) { unlinkFromTranslation(transformableObject); unlinkFromRotation(transformableObject); unlinkFromScale(transformableObject); unlinkFromRotationTranslation(transformableObject); unlinkFromScaleTranslation(transformableObject); }
	};
}