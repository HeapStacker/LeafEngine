#include "Colider.h"
#include "shader.h"
#include "model.h"

static float boxVertices[] = {
	// positions         
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

bool Colider::visibility = false;

std::vector<ColisionPair> Colider::colisionList;

void Colider::setVisibility(bool visible) {
	Colider::visibility = visible;
}

ColisionPair Colider::GetLatestColision() {
	if (!colisionList.empty()) {
		ColisionPair pair = colisionList.back();
		colisionList.pop_back();
		return pair;
	}
	return { 0, 0 };
}

static unsigned int boxColiderVAO;
static unsigned int boxColiderVBO;
static unsigned int boxVerticesCount = sizeof(boxVertices) / sizeof(float);
extern Shader* multiLightTextureShader;
//extern Shader* colorShader;
extern Shader* newColorShader;

glm::vec3 getPos(glm::mat4& mMatrix) {
	return glm::vec3(mMatrix[3]);
}

void setBoxColiderVAOVBO() {
	glGenVertexArrays(1, &boxColiderVAO);
	glGenBuffers(1, &boxColiderVBO);
	glBindBuffer(GL_ARRAY_BUFFER, boxColiderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
	glBindVertexArray(boxColiderVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void setShaderDrawProperties(Shader* shader, Camera* camera, glm::mat4& model, glm::mat4& view, glm::mat4& projection);

//void BoxColider::draw(Camera* camera, glm::mat4& view, glm::mat4& projection) {
//	if (visibility) {
//		setShaderDrawProperties(colorShader, camera, modelMatrix, view, projection);
//		colorShader->setVec3("color", { 0, 1, 0 });
//		glBindVertexArray(boxColiderVAO);
//		glDrawArrays(GL_TRIANGLES, 0, boxVerticesCount);
//	}
//}

void BoxColider::draw(Camera* camera, glm::mat4& view, glm::mat4& projection) {
	if (visibility) {
		setShaderDrawProperties(newColorShader, camera, modelMatrix, view, projection);
		newColorShader->setVec3("color", { 0, 1, 0 });
		glBindVertexArray(boxColiderVAO);
		glDrawArrays(GL_TRIANGLES, 0, boxVerticesCount);
	}
}

//void SphereColider::draw(Camera* camera, glm::mat4& view, glm::mat4& projection) {
//	if (visibility) {
//		static Model sphereModel = Model("sphere.obj");
//		setShaderDrawProperties(colorShader, camera, modelMatrix, view, projection);
//		colorShader->setVec3("color", { 0, 1, 0 });
//		sphereModel.Draw(*colorShader);
//	}
//}

void SphereColider::draw(Camera* camera, glm::mat4& view, glm::mat4& projection) {
	if (visibility) {
		static Model sphereModel = Model("sphere.obj");
		setShaderDrawProperties(newColorShader, camera, modelMatrix, view, projection);
		newColorShader->setVec3("color", { 0, 1, 0 });
		sphereModel.Draw(*newColorShader);
	}
}

void BoxColisionExecutor::isColision(BoxColider* stationaryColider) {
	static bool isColision = true;
	isColision = true;
	static std::vector<glm::vec3> axes = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 0, 1)
	};
	glm::vec3 delta = getPos(stationaryColider->modelMatrix) - getPos(transformedColider->modelMatrix);
	for (const auto& axis : axes) {
		float projection1 = glm::dot(getPos(transformedColider->modelMatrix), axis);
		float min1 = projection1 - glm::dot(transformedColider->halfExtents, glm::abs(axis));
		float max1 = projection1 + glm::dot(transformedColider->halfExtents, glm::abs(axis));
		float projection2 = glm::dot(getPos(stationaryColider->modelMatrix), axis);
		float min2 = projection2 - glm::dot(stationaryColider->halfExtents, glm::abs(axis));
		float max2 = projection2 + glm::dot(stationaryColider->halfExtents, glm::abs(axis));
		if (max1 < min2 || max2 < min1) isColision = false;
	}
	for (const auto& axis : axes) {
		float deltaProjection = glm::dot(delta, axis);
		float sumExtents = glm::dot(transformedColider->halfExtents, glm::abs(axis)) + glm::dot(stationaryColider->halfExtents, glm::abs(axis));
		if (glm::abs(deltaProjection) > sumExtents) isColision = false;
	}
	if (isColision) Colider::colisionList.push_back({ this->transformedColider->getLinkedObjectId(), stationaryColider->getLinkedObjectId() });
}

void BoxColisionExecutor::isColision(SphereColider* stationaryColider) {
	static bool isColision = true;
	isColision = true;
	static std::vector<glm::vec3> axes = {
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1)
	};
	glm::vec3 delta = getPos(stationaryColider->modelMatrix) - getPos(transformedColider->modelMatrix);
	for (const auto& axis : axes) {
		float projection1 = glm::dot(getPos(transformedColider->modelMatrix), axis);
		float min1 = projection1 - glm::dot(transformedColider->halfExtents, glm::abs(axis));
		float max1 = projection1 + glm::dot(transformedColider->halfExtents, glm::abs(axis));
		float projection2 = glm::dot(getPos(stationaryColider->modelMatrix), axis);
		float min2 = projection2 - glm::dot(glm::vec3(stationaryColider->radius), glm::abs(axis));
		float max2 = projection2 + glm::dot(glm::vec3(stationaryColider->radius), glm::abs(axis));
		if (max1 < min2 || max2 < min1) isColision = false;
	}
	for (const auto& axis : axes) {
		float deltaProjection = glm::dot(delta, axis);
		float sumExtents = glm::dot(transformedColider->halfExtents, glm::abs(axis)) + glm::dot(glm::vec3(stationaryColider->radius), glm::abs(axis));
		if (glm::abs(deltaProjection) > sumExtents) isColision = false;
	}
	if (isColision) Colider::colisionList.push_back({ this->transformedColider->getLinkedObjectId(), stationaryColider->getLinkedObjectId() });
}


void SphereColisionExecutor::isColision(SphereColider* stationaryColider) {
	glm::vec3 delta = getPos(stationaryColider->modelMatrix) - getPos(transformedColider->modelMatrix);
	if (sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) <= stationaryColider->radius + transformedColider->radius) 
		Colider::colisionList.push_back({ this->transformedColider->getLinkedObjectId(), stationaryColider->getLinkedObjectId() });
}

void SphereColisionExecutor::isColision(BoxColider* stationaryColider) {
	static bool isColision = true;
	isColision = true;
	static std::vector<glm::vec3> axes = {
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1)
	};
	glm::vec3 delta = getPos(stationaryColider->modelMatrix) - getPos(transformedColider->modelMatrix);
	for (const auto& axis : axes) {
		float projection1 = glm::dot(getPos(transformedColider->modelMatrix), axis);
		float min1 = projection1 - glm::dot(glm::vec3(transformedColider->radius), glm::abs(axis));
		float max1 = projection1 + glm::dot(glm::vec3(transformedColider->radius), glm::abs(axis));
		float projection2 = glm::dot(getPos(stationaryColider->modelMatrix), axis);
		float min2 = projection2 - glm::dot(stationaryColider->halfExtents, glm::abs(axis));
		float max2 = projection2 + glm::dot(stationaryColider->halfExtents, glm::abs(axis));
		if (max1 < min2 || max2 < min1) isColision = false;
	}
	for (const auto& axis : axes) {
		float deltaProjection = glm::dot(delta, axis);
		float sumExtents = glm::dot(glm::vec3(transformedColider->radius), glm::abs(axis)) + glm::dot(stationaryColider->halfExtents, glm::abs(axis));
		if (glm::abs(deltaProjection) > sumExtents) isColision = false;
	}
	if (isColision) Colider::colisionList.push_back({ this->transformedColider->getLinkedObjectId(), stationaryColider->getLinkedObjectId() });
}

void Colision::DealWithColisions() {
	static ColisionPair colisionPair, lastColisionPair;
	while (true) {
		colisionPair = Colider::GetLatestColision();
		//ensures that 2 same, stacked collisions don't repeat
		if (!colisionPair.equalTo(lastColisionPair)) {
			DealWithColision(colisionPair);
			lastColisionPair = colisionPair;
		}
		else break;
	}
}