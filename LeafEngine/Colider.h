#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "manualSettings.h"

class BoxColider;
class SphereColider;

static unsigned int boxColiderVAO;
static unsigned int boxColiderVBO;
static unsigned int boxVerticesCount = sizeof(vertices) / sizeof(float);
static unsigned int sphereColiderVAO;
static unsigned int sphereColiderVBO;
static unsigned int sphereVerticesCount = 0; //not implemented yet

struct VisualColiderData {
	unsigned int coliderVAO;
	unsigned int verticesCount;
};

static void setBoxColiderVAOVBO() {
	glGenVertexArrays(1, &boxColiderVAO);
	glGenBuffers(1, &boxColiderVBO);
	glBindBuffer(GL_ARRAY_BUFFER, boxColiderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
	glBindVertexArray(boxColiderVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

static void setSphereColiderVAOVBO() {
	std::cerr << "Sphere colider representation not implemented yet\n";
	exit(4);
}

class ColisionExecutor {
public:
	virtual bool isColision(BoxColider* colider) = 0;
	virtual bool isColision(SphereColider* colider) = 0;
};

class Colider {
public:
	glm::mat4* linkedModelMatrix = nullptr;

	Colider(glm::mat4* linkModelMatrix) {
		if (linkModelMatrix) linkedModelMatrix = linkModelMatrix;
		else {
			std::cerr << "Linked Model Matrix = nullptr!\n";
			exit(3);
		}
	}

	virtual void acceptColisionExecution(ColisionExecutor* colisionExecutor) = 0;
	virtual VisualColiderData getVisualColiderData() = 0;
};

class BoxColider : public Colider {
public:
	glm::vec3 halfExtents;
	BoxColider(glm::mat4* linkModelMatrix, const glm::vec3& sides)
		:Colider(linkModelMatrix)
	{
		static bool firstInit = true;
		if (firstInit) { setBoxColiderVAOVBO(); firstInit = false; }
		halfExtents = sides / 2.f;
	}

	void acceptColisionExecution(ColisionExecutor* colisionExecutor) override {
		if (colisionExecutor->isColision(this)) {
			std::cout << "Colision occurred.\n";
		}
	}

	VisualColiderData getVisualColiderData() override { return { boxColiderVAO, boxVerticesCount }; }
};

class SphereColider : public Colider {
public:
	float radius;

	SphereColider(glm::mat4* linkModelMatrix, float radius) :Colider(linkModelMatrix) {
		static bool firstInit = true;
		if (firstInit) { setSphereColiderVAOVBO; firstInit = false; }
		this->radius = radius;
	}

	void acceptColisionExecution(ColisionExecutor* colisionExecutor) override {
		if (colisionExecutor->isColision(this)) {
			std::cout << "Colision occurred.\n";
		}
	}

	VisualColiderData getVisualColiderData() override { return { sphereColiderVAO, sphereVerticesCount }; }
};

glm::vec3 getPos(glm::mat4& mMatrix);

class BoxColisionExecutor : public ColisionExecutor {
	BoxColider* transformedColider = nullptr;
public:
	BoxColisionExecutor(BoxColider* transformedColider) {
		if (transformedColider) this->transformedColider = transformedColider;
		else {
			std::cerr << "Accepted box colider = nullptr!\n";
			exit(3);
		}
	}

	bool isColision(BoxColider* stationaryColider) override {
		std::vector<glm::vec3> axes = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 0, 1)
		};
		glm::vec3 delta = getPos(*stationaryColider->linkedModelMatrix) - getPos(*transformedColider->linkedModelMatrix);
		for (const auto& axis : axes) {
			float projection1 = glm::dot(getPos(*transformedColider->linkedModelMatrix), axis);
			float min1 = projection1 - glm::dot(transformedColider->halfExtents, glm::abs(axis));
			float max1 = projection1 + glm::dot(transformedColider->halfExtents, glm::abs(axis));
			float projection2 = glm::dot(getPos(*stationaryColider->linkedModelMatrix), axis);
			float min2 = projection2 - glm::dot(stationaryColider->halfExtents, glm::abs(axis));
			float max2 = projection2 + glm::dot(stationaryColider->halfExtents, glm::abs(axis));
			if (max1 < min2 || max2 < min1) {
				return false;
			}
		}
		for (const auto& axis : axes) {
			float deltaProjection = glm::dot(delta, axis);
			float sumExtents = glm::dot(transformedColider->halfExtents, glm::abs(axis)) + glm::dot(stationaryColider->halfExtents, glm::abs(axis));
			if (glm::abs(deltaProjection) > sumExtents) {
				return false;
			}
		}
		return true;
	}

	bool isColision(SphereColider* stationaryColider) override {
		std::cerr << "Not implemented yet\n";
		return false;
	}
};

class SphereColisionExecutor : public ColisionExecutor {
	SphereColider* transformedColider = nullptr;
public:
	SphereColisionExecutor(SphereColider* transformedColider) {
		if (transformedColider) this->transformedColider = transformedColider;
		else {
			std::cerr << "Accepted box colider = nullptr!\n";
			exit(3);
		}
	}

	bool isColision(BoxColider* stationaryColider) override {
		std::cerr << "Not implemented yet\n";
		return false;
	}

	bool isColision(SphereColider* stationaryColider) override {
		glm::vec3 delta = getPos(*stationaryColider->linkedModelMatrix) - getPos(*transformedColider->linkedModelMatrix);
		if (sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) <= stationaryColider->radius + transformedColider->radius) return true;
		return false;
	}
};