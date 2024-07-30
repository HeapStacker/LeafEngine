#include "TransformableObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace lf {

	TransformableObject::TransformableObject()
	{
		linkedModelMatrixS = &modelMatrix;
		linkedModelMatrixR = &modelMatrix;
		linkedModelMatrixM = &modelMatrix;
	}

	void TransformableObject::linkToRotation(TransformableObject* object)
	{
		object->linkedObjects.push_back(this);
	}

    void TransformableObject::rotateAround(glm::vec3 axis, float degrees)
    {
        this->RotateableObject::rotateAround(axis, degrees);
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
        for (TransformableObject* obj : linkedObjects) {
            obj->TransformableObject::rotateAround(axis, degrees);
            glm::vec3 distanceVector = obj->getPosition() - this->getPosition();
            glm::vec4 rotatedDistance = rotationMatrix * glm::vec4(distanceVector, 1.0f);
            obj->setPosition(this->getPosition() + glm::vec3(rotatedDistance));
        }
    }

}
