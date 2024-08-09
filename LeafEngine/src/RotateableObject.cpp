#include "RotateableObject.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace lf {
    glm::vec3&& RotateableObject::getRotation()
    {
        glm::mat3 rotationMatrix = glm::mat3(modelMatrix);
        rotationMatrix[0] = glm::normalize(rotationMatrix[0]);
        rotationMatrix[1] = glm::normalize(rotationMatrix[1]);
        rotationMatrix[2] = glm::normalize(rotationMatrix[2]);
        glm::quat rotationQuat = glm::quat_cast(rotationMatrix);
        glm::vec3 eulerAngles = glm::eulerAngles(rotationQuat);
        return glm::degrees(eulerAngles);
    }

    std::vector<glm::vec3> RotateableObject::getAxes() {
        glm::vec3 xAxis = glm::vec3(modelMatrix[0]); // Prvi stupac
        glm::vec3 yAxis = glm::vec3(modelMatrix[1]); // Drugi stupac
        glm::vec3 zAxis = glm::vec3(modelMatrix[2]); // Treæi stupac
        xAxis = glm::normalize(xAxis);
        yAxis = glm::normalize(yAxis);
        zAxis = glm::normalize(zAxis);
        return { xAxis, yAxis, zAxis };
    }

    void RotateableObject::orient(const glm::vec3& orientation) {
        glm::vec3 radians = glm::radians(orientation);
        glm::quat rotationQuat = glm::quat(radians);
        glm::mat3 rotationMatrix = glm::toMat3(rotationQuat);
        glm::vec3 translation, scale, skew;
        glm::vec4 perspective;
        glm::quat oldRotation;
        glm::decompose(modelMatrix, scale, oldRotation, translation, skew, perspective);
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, translation);
        modelMatrix *= glm::mat4(rotationMatrix);
        modelMatrix = glm::scale(modelMatrix, scale);
    }

    void RotateableObject::rotateOnly(glm::vec3 axis, float degrees)
    {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), glm::normalize(axis));
        this->RotateableObject::traversed = true;
        for (RotateableObject* obj : linkedToRotation) {
            if (!obj->RotateableObject::traversed) obj->RotateableObject::rotateOnly(axis, degrees);
        }
        this->RotateableObject::traversed = false;
    }

    void RotateableObject::linkToRotation(RotateableObject* object)
    {
        if (object) {
            object->linkedToRotation.push_back(this);
            this->linkedToRotation.push_back(object);
        }
    }

    void RotateableObject::unlinkFromRotation(RotateableObject* object)
    {
        if (object) {
            object->linkedToRotation.erase(std::remove(object->linkedToRotation.begin(), object->linkedToRotation.end(), this), object->linkedToRotation.end());
            this->linkedToRotation.erase(std::remove(this->linkedToRotation.begin(), this->linkedToRotation.end(), object), this->linkedToRotation.end());
        }
    }

}