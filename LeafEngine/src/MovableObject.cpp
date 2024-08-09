#include "MovableObject.h"

namespace lf {
    void MovableObject::setPosition(const glm::vec3& position)
    {
        modelMatrix[3] = glm::vec4({ position, 1 });
    }

    void MovableObject::setPosition(MovableObject* object)
    {
        modelMatrix[3] = object->modelMatrix[3];
    }

    glm::vec3&& MovableObject::getPosition()
    {
        return glm::vec3(modelMatrix[3]);
    }

    void MovableObject::translate(const glm::vec3& position)
    {
        modelMatrix[3] = glm::vec4({ glm::vec3(modelMatrix[3]) + position, 1 });
        this->MovableObject::traversed = true;
        for (MovableObject* obj : linkedToTranslation) {
            if (!obj->MovableObject::traversed) obj->translate(position);
        }
        this->MovableObject::traversed = false;
    }

    void MovableObject::linkToTranslation(MovableObject* object)
    {
        if (object) {
            object->linkedToTranslation.push_back(this);
            this->linkedToTranslation.push_back(object);
        }
    }

    void MovableObject::unlinkFromTranslation(MovableObject* object)
    {
        if (object) {
            object->linkedToTranslation.erase(std::remove(object->linkedToTranslation.begin(), object->linkedToTranslation.end(), this), object->linkedToTranslation.end());
            this->linkedToTranslation.erase(std::remove(this->linkedToTranslation.begin(), this->linkedToTranslation.end(), object), this->linkedToTranslation.end());
        }
    }
}
