#include "MobileScalableObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace lf {
	void MobileScalableObject::scaleTranslate(float scalar)
	{
        SpatialObject::modelMatrix = glm::scale(SpatialObject::modelMatrix, glm::vec3(scalar));
        this->MobileScalableObject::traversed = true;
        for (MobileScalableObject* obj : linkedToScaleTranslation) {
            if (!obj->MobileScalableObject::traversed) {
                obj->scaleTranslate(scalar);
                glm::vec3 distanceVector = obj->getPosition() - this->getPosition();
                obj->setPosition(this->getPosition() + distanceVector * scalar);
            }
        }
        this->MobileScalableObject::traversed = false;
	}

    void MobileScalableObject::scaleTranslate(const glm::vec3& scalar)
    {
        SpatialObject::modelMatrix = glm::scale(SpatialObject::modelMatrix, scalar);
        this->MobileScalableObject::traversed = true;
        for (MobileScalableObject* obj : linkedToScaleTranslation) {
            if (!obj->MobileScalableObject::traversed) {
                obj->scaleTranslate(scalar);
                glm::vec3 distanceVector = obj->getPosition() - this->getPosition();
                obj->setPosition(this->getPosition() + distanceVector * scalar);
            }
        }
        this->MobileScalableObject::traversed = false;
    }

    void MobileScalableObject::linkToScaleTranslation(MobileScalableObject* object)
    {
        if (object) {
            object->linkedToScaleTranslation.push_back(this);
            this->linkedToScaleTranslation.push_back(object);
        }
    }

    void MobileScalableObject::unlinkFromScaleTranslation(MobileScalableObject* object)
    {
        if (object) {
            object->linkedToScaleTranslation.erase(std::remove(object->linkedToScaleTranslation.begin(), object->linkedToScaleTranslation.end(), this), object->linkedToScaleTranslation.end());
            this->linkedToScaleTranslation.erase(std::remove(this->linkedToScaleTranslation.begin(), this->linkedToScaleTranslation.end(), object), this->linkedToScaleTranslation.end());
        }
    }
}