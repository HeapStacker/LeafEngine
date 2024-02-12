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
	bool equalTo(const ColisionPair& colisionPair);
};

class Colider {
	unsigned int linkedObjectId = 0;
protected:
	static bool visibility;
public:
	static std::vector<ColisionPair> colisionList;
	glm::mat4 modelMatrix;

	Colider(const glm::mat4& setModelMatrix, unsigned int objectId);
	virtual void acceptColisionExecution(ColisionExecutor* colisionExecutor) = 0;
	unsigned int getLinkedObjectId();
	void setPosition(const glm::vec3& position);
	void translate(const glm::vec3& position);
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

	BoxColider(glm::mat4& linkModelMatrix, unsigned int objectId);
	void acceptColisionExecution(ColisionExecutor* colisionExecutor) override;
	void scale(float scalar) override;
	void scale(const glm::vec3& scalar) override;
	void rotateAround(glm::vec3 axis, float degrees) override;
	void Colider::draw(Camera* camera, glm::mat4& view, glm::mat4& projection) override;
};

class SphereColider : public Colider {
public:
	float radius;

	SphereColider(glm::mat4& linkModelMatrix, unsigned int objectId);
	void acceptColisionExecution(ColisionExecutor* colisionExecutor) override;
	void scale(float scalar) override;
	void scale(const glm::vec3& scalar) override;
	void rotateAround(glm::vec3 axis, float degrees) override;
	void draw(Camera* camera, glm::mat4& view, glm::mat4& projection) override;
};

class BoxColisionExecutor : public ColisionExecutor {
	BoxColider* transformedColider = nullptr;
public:
	BoxColisionExecutor(BoxColider* transformedColider);
	void isColision(BoxColider* stationaryColider) override;
	void isColision(SphereColider* stationaryColider) override; //approximate sphere as a box
};

class SphereColisionExecutor : public ColisionExecutor {
	SphereColider* transformedColider = nullptr;
public:
	SphereColisionExecutor(SphereColider* transformedColider);
	void isColision(SphereColider* stationaryColider) override;
	void isColision(BoxColider* stationaryColider) override; //approximate sphere as a box
};

class Colision {
protected:
	static void DealWithColisions();
public:
	static void DealWithColision(ColisionPair& pair);
};