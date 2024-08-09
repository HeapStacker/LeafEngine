#pragma once
#include "ScalableObject.h"
#include "MobileScalableObject.h"
#include "MobileRotatableObject.h"

namespace lf {
	class TransformableObject : public MobileRotatableObject, public MobileScalableObject {
	public:
		void linkTo(TransformableObject* transformableObject) { MovableObject::linkToTranslation(transformableObject); linkToRotationTranslation(transformableObject); linkToScaleTranslation(transformableObject); }
		void unlinkFrom(TransformableObject* transformableObject) { MovableObject::unlinkFromTranslation(transformableObject); unlinkFromRotationTranslation(transformableObject); unlinkFromScaleTranslation(transformableObject); }
	};
}