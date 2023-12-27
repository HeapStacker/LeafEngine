#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"


class BoxColider;
class SphereColider;

class ColisionExecutor {
public:
	virtual void isColision(BoxColider* colider) = 0;
	virtual void isColision(SphereColider* colider) = 0;
};

void setBoxColiderVAOVBO();
glm::vec3 getPos(glm::mat4& mMatrix);

struct ColisionPair {
	unsigned int transformedColider;
	unsigned int targetedColider;
	bool equalTo(const ColisionPair& colisionPair) {
		return colisionPair.targetedColider == targetedColider && colisionPair.transformedColider == transformedColider;
	}
};

class Colider {
	unsigned int linkedObjectId = 0;
protected:
	static bool visibility;
public:
	static std::vector<ColisionPair> colisionList;
	glm::mat4 modelMatrix;

	Colider(const glm::mat4& setModelMatrix, unsigned int objectId) {
		linkedObjectId = objectId;
		modelMatrix = setModelMatrix;
	}

	virtual void acceptColisionExecution(ColisionExecutor* colisionExecutor) = 0;

	unsigned int getLinkedObjectId() { return linkedObjectId; }

	void setPosition(const glm::vec3& position) {
		modelMatrix = glm::mat4(1.f);
		modelMatrix = glm::translate(modelMatrix, position);
	}

	void translate(const glm::vec3& position) {
		this->setPosition(glm::vec3(modelMatrix[3]) + position);
	}

	static void setVisibility(bool visible);
	static ColisionPair GetLatestColision();
	virtual void scale(float scalar) = 0;
	virtual void scale(const glm::vec3& scalar) = 0;
	virtual void rotateAround(glm::vec3 axis, float degrees) = 0;
	virtual void draw(Camera* camera, glm::mat4& view, glm::mat4& projection) = 0;
};

class BoxColider : public Colider {
public:
	glm::vec3 halfExtents;
	BoxColider(glm::mat4& linkModelMatrix, unsigned int objectId)
		:Colider(glm::scale(linkModelMatrix, {1, 1, 1}), objectId)
	{
		static bool firstInit = true;
		if (firstInit) { setBoxColiderVAOVBO(); firstInit = false; }
		halfExtents = glm::vec3(1, 1, 1) / 2.f;
	}

	void acceptColisionExecution(ColisionExecutor* colisionExecutor) override {
		colisionExecutor->isColision(this);
	}

	void scale(float scalar) override {
		halfExtents *= scalar;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scalar));
	}

	void scale(const glm::vec3& scalar) override {
		halfExtents *= scalar;
		modelMatrix = glm::scale(modelMatrix, scalar);
	}

	void rotateAround(glm::vec3 axis, float degrees) override {
		modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), glm::normalize(axis));
	}

	void Colider::draw(Camera* camera, glm::mat4& view, glm::mat4& projection) override;
};

class SphereColider : public Colider {
public:
	float radius;

	SphereColider(glm::mat4& linkModelMatrix, unsigned int objectId)
		:Colider(glm::scale(linkModelMatrix, glm::vec3(1)), objectId)
	{
		this->radius = 1;
	}

	void acceptColisionExecution(ColisionExecutor* colisionExecutor) override {
		colisionExecutor->isColision(this);
	}

	void scale(float scalar) override {
		radius *= scalar;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scalar));
	}

	void scale(const glm::vec3& scalar) override {
		std::cerr << "Cant scale sphere with vector scalar.\n";
	}

	void rotateAround(glm::vec3 axis, float degrees) override {
		std::cerr << "You cant really rotate an uniform sphere.\n";
	}

	void draw(Camera* camera, glm::mat4& view, glm::mat4& projection) override;
};

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

	void isColision(BoxColider* stationaryColider) override;
	void isColision(SphereColider* stationaryColider) override; //approximate sphere as a box
};

//danas sam prekinul s Laurom (16.12.2023.) :c

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

	void isColision(SphereColider* stationaryColider) override;
	void isColision(BoxColider* stationaryColider) override; //approximate sphere as a box
};

static class Colision {
protected:
	static void DealWithColisions();
public:
	static void DealWithColision(ColisionPair& pair);
};