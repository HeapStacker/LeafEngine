#include "Collider.h"
#include "ShaderImplementation.h"
#include "Camera.h"

namespace lf {
	static float boxVertices[] = {    
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

	static unsigned int boxColiderVAO;
	static unsigned int boxColiderVBO;
	static unsigned int boxVerticesCount = sizeof(boxVertices) / sizeof(float);
	static std::vector<Collider*> colliders;
	static unsigned int boxColiderCount = 0;

	static std::vector<ColisionPair> colisions;

	bool Collider::isColisionRegistered(Collider* col1, Collider* col2)
	{
		for (ColisionPair& p : colisions) {
			if (col1->id == p.id1 && col2->id == p.id2 || col1->id == p.id2 && col2->id == p.id1) return true;
		}
		return false;
	}

	static void setBoxColiderVAOVBO() {
		glGenVertexArrays(1, &boxColiderVAO);
		glGenBuffers(1, &boxColiderVBO);
		glBindBuffer(GL_ARRAY_BUFFER, boxColiderVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
		glBindVertexArray(boxColiderVAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void Collider::setNewId()
	{
		id = boxColiderCount++;
	}

	Collider::Collider()
	{
		setNewId();
		static bool firstInit = true;
		if (firstInit) { setBoxColiderVAOVBO(); firstInit = false; }
		halfExtents = glm::vec3(1, 1, 1) / 2.f;
		colliders.push_back(this);
	}

	void Collider::scale(float scalar) {
		halfExtents *= scalar;
		scale(scalar);
	}

	void Collider::scale(const glm::vec3& scalar) {
		halfExtents *= scalar;
		scale(scalar);
	}

	bool Collider::colidesWith(Collider* colider)
	{
		static bool isColision;
		isColision = true;
		static std::vector<glm::vec3> axes = {
				glm::vec3(1, 0, 0),
				glm::vec3(0, 1, 0),
				glm::vec3(0, 0, 1)
		};
		glm::vec3 delta = colider->getPosition() - this->getPosition();
		for (glm::vec3& axis : axes) {
			float deltaProjection = glm::dot(delta, axis);
			float sumExtents = glm::dot(this->halfExtents, axis) + glm::dot(colider->halfExtents, axis);
			if (glm::abs(deltaProjection) > sumExtents) isColision = false;
		}
		if (isColision) return true;
		return false;
	}

	bool Collider::operator==(const Collider& colider)
	{
		return this->id == colider.id;
	}

	bool Collider::operator!=(const Collider& colider)
	{
		return this->id != colider.id;
	}

	static bool visible = false;

	void Collider::CheckForCollisions()
	{
		for (Collider* collider1 : colliders) {
			for (Collider* collider2 : colliders) {
				if (collider1 != collider2 && !isColisionRegistered(collider1, collider2)) {
					if (collider1->colidesWith(collider2)) {
						colisions.push_back({ collider1->id, collider2->id });
					}
				}
			}
		}
	}

	static ColisionPair getLastCollision()
	{
		if (!colisions.empty()) {
			ColisionPair pair = colisions.back();
			colisions.pop_back();
			return pair;
		}
		return { 0, 0 };
	}

	static ColisionFunction collisionRule;

	bool areEqual(ColisionPair* cp1, ColisionPair* cp2) {
		return cp1->id1 == cp2->id1 && cp1->id2 == cp2->id2 || cp1->id1 == cp2->id2 && cp1->id2 == cp2->id1;
	}

	void Collider::ResolveCollisions()
	{
		static ColisionPair nullCp = { 0, 0 };
		static ColisionPair cp, lastCp = { 0, 0 };
		while (true) {
			cp = getLastCollision();
			//ensures that 2 same, stacked collisions don't repeat
			if (!areEqual(&cp, &lastCp)) {
				if (!areEqual(&cp, &nullCp)) collisionRule(cp);
				lastCp = cp;
			}
			else break;
		}
	}

	void Collider::AssignCollisionRules(ColisionFunction collisionRuleFunction)
	{
		collisionRule = collisionRuleFunction;
	}

	void Collider::SetVisibility(bool visibility)
	{
		visible = visibility;
	}

	static Shader& coloredShader = getColoredShader();

	void Collider::render() {
		coloredShader.setMat4("model", modelMatrix);
		coloredShader.setVec3("color", { 0, 1, 0 });
		glBindVertexArray(boxColiderVAO);
		glDrawArrays(GL_TRIANGLES, 0, boxVerticesCount);
	}

	void Collider::RenderColliders(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
	{
		if (visible) {
			setShaderDrawProperties(&coloredShader, Camera::GetActiveCamera(), viewMatrix, projectionMatrix);
			for (Collider* col : colliders) {
				col->render();
			}
		}
	}
}