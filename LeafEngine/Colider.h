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
	virtual bool isColision(BoxColider* colider) = 0;
	virtual bool isColision(SphereColider* colider) = 0;
};

void setBoxColiderVAOVBO();
glm::vec3 getPos(glm::mat4& mMatrix);

class Colider {
protected:
	static bool visibility;
public:
	glm::mat4 modelMatrix;

	Colider(const glm::mat4& setModelMatrix) {
		modelMatrix = setModelMatrix;
	}

	virtual void acceptColisionExecution(ColisionExecutor* colisionExecutor) = 0;

	void setPosition(const glm::vec3& position) {
		modelMatrix = glm::mat4(1.f);
		modelMatrix = glm::translate(modelMatrix, position);
	}

	void translate(const glm::vec3& position) {
		modelMatrix = glm::translate(modelMatrix, getPos(modelMatrix) + position);
	}

	static void setVisibility(bool visible);
	virtual void scale(float scalar) = 0;
	virtual void scale(const glm::vec3& scalar) = 0;
	virtual void rotateAround(glm::vec3 axis, float degrees) = 0;
	virtual void draw(Camera* camera, glm::mat4& view, glm::mat4& projection) = 0;
};

class BoxColider : public Colider {
public:
	glm::vec3 halfExtents;
	BoxColider(glm::mat4& linkModelMatrix, const glm::vec3& sides)
		:Colider(glm::scale(linkModelMatrix, sides))
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

	SphereColider(glm::mat4& linkModelMatrix, float radius) 
		:Colider(glm::scale(linkModelMatrix, glm::vec3(radius)))
	{
		this->radius = radius;
	}

	void acceptColisionExecution(ColisionExecutor* colisionExecutor) override {
		if (colisionExecutor->isColision(this)) {
			std::cout << "Colision occurred.\n";
		}
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

	bool isColision(BoxColider* stationaryColider) override;
	bool isColision(SphereColider* stationaryColider) override; //approximate sphere as a box
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

	bool isColision(SphereColider* stationaryColider) override;
	bool isColision(BoxColider* stationaryColider) override; //approximate sphere as a box
};