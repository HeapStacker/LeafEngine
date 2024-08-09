#include "MobileRotatableObject.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace lf {
    void MobileRotatableObject::rotate(glm::vec3 axis, float degrees)
    {
        SpatialObject::modelMatrix = glm::rotate(SpatialObject::modelMatrix, glm::radians(degrees), glm::normalize(axis));
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
        this->MobileRotatableObject::traversed = true;
        for (MobileRotatableObject* obj : linkedToRotationTranslation) {
            if (!obj->MobileRotatableObject::traversed) {
                obj->rotate(axis, degrees);
                glm::vec3 distanceVector = obj->getPosition() - this->getPosition();
                glm::vec4 rotatedDistance = rotationMatrix * glm::vec4(distanceVector, 1.0f);
                obj->setPosition(this->getPosition() + glm::vec3(rotatedDistance));
            }
        }
        this->MobileRotatableObject::traversed = false;
    }

    void MobileRotatableObject::linkToRotationTranslation(MobileRotatableObject* object)
    {
        if (object) {
            object->linkedToRotationTranslation.push_back(this);
            this->linkedToRotationTranslation.push_back(object);
        }
    }

    void MobileRotatableObject::unlinkFromRotationTranslation(MobileRotatableObject* object)
    {
        if (object) {
            object->linkedToRotationTranslation.erase(std::remove(object->linkedToRotationTranslation.begin(), object->linkedToRotationTranslation.end(), this), object->linkedToRotationTranslation.end());
            this->linkedToRotationTranslation.erase(std::remove(this->linkedToRotationTranslation.begin(), this->linkedToRotationTranslation.end(), object), this->linkedToRotationTranslation.end());
        }
    }

}
