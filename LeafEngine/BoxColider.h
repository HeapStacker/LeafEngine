#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>

class BoxColider
{
	glm::mat4 modelMatrix = glm::mat4(1.f);
	glm::vec3 halfExtents = { 0, 0, 0 };
public:
	BoxColider(glm::mat4& modelMatrix, const glm::vec3& halfExtents) {
		this->modelMatrix = modelMatrix;
		this->halfExtents = halfExtents;
	}

    //bool checkCollision(BoxColider* colider) {
    //    // Transformacija kvadra 1 u svijet
    //    glm::mat4 transform1 = this->modelMatrix * glm::mat4_cast(glm::quat_cast(glm::mat3(this->modelMatrix))) * glm::scale(glm::mat4(1.0f), this->halfExtents);

    //    // Transformacija kvadra 2 u svijet
    //    glm::mat4 transform2 = colider->modelMatrix * glm::mat4_cast(glm::quat_cast(glm::mat3(colider->modelMatrix))) * glm::scale(glm::mat4(1.0f), colider->halfExtents);

    //    // Radi lakšeg pristupa osnovnim vektorima
    //    glm::vec3 axes[3] = { glm::vec3(transform1[0]), glm::vec3(transform1[1]), glm::vec3(transform1[2]) };

    //    // Razlika izmeðu središta kvadara
    //    glm::vec3 delta = glm::vec3(transform2[3]) - glm::vec3(transform1[3]);

    //    for (int i = 0; i < 3; i++) {
    //        // Projektirajte kvadar 1 i kvadar 2 na trenutnu os
    //        float extent1 = this->halfExtents[i];
    //        float extent2 = colider->halfExtents[i];
    //        float projection = glm::abs(glm::dot(axes[i], delta));

    //        // Provjerite koliziju na trenutnoj osi
    //        if (projection > extent1 + extent2) {
    //            return false;  // Nema kolizije
    //        }
    //    }

    //    return true;  // Kolizija
    //}

};

