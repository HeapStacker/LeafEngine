#include "ScalableObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace lf {
    glm::vec3 ScalableObject::getScale() {
        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(modelMatrix[0]));
        scale.y = glm::length(glm::vec3(modelMatrix[1]));
        scale.z = glm::length(glm::vec3(modelMatrix[2]));
        return scale;
    }

    void ScalableObject::scale(float scalar)
    {
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scalar));
        this->ScalableObject::traversed = true;
        for (ScalableObject* obj : linkedToScale) {
            if (!obj->ScalableObject::traversed) obj->scale(scalar);
        }
        this->ScalableObject::traversed = false;
    }

    void ScalableObject::scale(const glm::vec3& scalar)
    {
        modelMatrix = glm::scale(modelMatrix, scalar);
        this->ScalableObject::traversed = true;
        for (ScalableObject* obj : linkedToScale) {
            if (!obj->ScalableObject::traversed) obj->scale(scalar);
        }
        this->ScalableObject::traversed = false;
    }

    void ScalableObject::linkToScale(ScalableObject* object)
    {
        if (object) {
            object->linkedToScale.push_back(this);
            this->linkedToScale.push_back(object);
        }
    }

    void ScalableObject::unlinkFromScale(ScalableObject* object)
    {
        if (object) {
            object->linkedToScale.erase(std::remove(object->linkedToScale.begin(), object->linkedToScale.end(), this), object->linkedToScale.end());
            this->linkedToScale.erase(std::remove(this->linkedToScale.begin(), this->linkedToScale.end(), object), this->linkedToScale.end());
        }
    }
}