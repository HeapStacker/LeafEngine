#include "MoveableObject.h"
#include <iostream>
#include <algorithm>
#include "PointLight.h"
#include "SpotLight.h"

namespace lf {

	void MoveableObject::setPosition(const glm::vec3& position) {
		this->position = position;
		(*linkedModelMatrixM)[3] = glm::vec4({ this->position, 1 });
	}

	glm::vec3 MoveableObject::getPosition()
	{
		return this->position;
	}

	void MoveableObject::translate(const glm::vec3& position)
	{
		this->position += position;
		(*linkedModelMatrixM)[3] = glm::vec4({ this->position, 1 });
		for (MoveableObject* obj : linkedObjects) {
			obj->translate(position);
		}
		if (isPointLight) PointLight::UpdatePointLights();
		if (isSpotLight) SpotLight::UpdateSpotLights();
	}

	void MoveableObject::linkToPosition(MoveableObject* object)
	{
		object->linkedObjects.push_back(this);
	}

	void MoveableObject::unlinkFromPosition(MoveableObject* object)
	{
		linkedObjects.erase(std::remove(linkedObjects.begin(), linkedObjects.end(), object), linkedObjects.end());
		linkedObjects.shrink_to_fit();
	}

}
