#include "Collider.h"
#include "Shapes.h"
#include "Shader.h"
#include "Camera.h"

namespace lf {
	static std::vector<Collider*> colliders;
	static unsigned int IdReff = 1;

	static std::vector<ColisionPair> colisions;

	bool Collider::isColisionRegistered(Collider* col1, Collider* col2)
	{
		for (ColisionPair& p : colisions) {
			if (col1->id == p.id1 && col2->id == p.id2 || col1->id == p.id2 && col2->id == p.id1) return true;
		}
		return false;
	}

	Collider::Collider()
	{
		this->id = IdReff++;
		initializeMinimalBoxVertices();
		colliders.push_back(this);
	}

	// extended Separating Axis Theorem (SAT)
	bool Collider::colidesWith(Collider* colider)
	{
		static glm::vec3 thisHalfExtents, coliderHalfExtents;
		thisHalfExtents = this->getScale() / 2.f;
		coliderHalfExtents = colider->getScale() / 2.f;
		static std::vector<glm::vec3> axes;
		axes.clear();
		std::vector<glm::vec3> thisAxes = this->getAxes();
		std::vector<glm::vec3> otherAxes = colider->getAxes();
		axes.insert(axes.end(), thisAxes.begin(), thisAxes.end());
		axes.insert(axes.end(), otherAxes.begin(), otherAxes.end());
		for (const auto& thisAxis : thisAxes) {
			for (const auto& otherAxis : otherAxes) {
				glm::vec3 crossProduct = glm::cross(thisAxis, otherAxis);
				if (glm::length(crossProduct) > 1e-6) { // Dodaj samo osi koje nisu preblizu nuli
					axes.push_back(glm::normalize(crossProduct));
				}
			}
		}
		glm::vec3 delta = colider->getPosition() - this->getPosition();
		for (const glm::vec3& axis : axes) {
			if (glm::length(axis) < 1e-6) continue;
			float deltaProjection = glm::dot(delta, axis);
			float thisExtent =
				std::abs(glm::dot(thisHalfExtents.x * thisAxes[0], axis)) +
				std::abs(glm::dot(thisHalfExtents.y * thisAxes[1], axis)) +
				std::abs(glm::dot(thisHalfExtents.z * thisAxes[2], axis));
			float otherExtent =
				std::abs(glm::dot(coliderHalfExtents.x * otherAxes[0], axis)) +
				std::abs(glm::dot(coliderHalfExtents.y * otherAxes[1], axis)) +
				std::abs(glm::dot(coliderHalfExtents.z * otherAxes[2], axis));
			float sumExtents = thisExtent + otherExtent;
			if (std::abs(deltaProjection) > sumExtents) return false;
		}
		return true;
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

	static Shader& coloredShader = Shader::getColoredShader();

	void Collider::render() {
		coloredShader.setMat4("model", this->SpatialObject::modelMatrix);
		coloredShader.setVec3("color", { 0, 1, 0 });
		glBindVertexArray(getMinimalBoxVao());
		glDrawArrays(GL_TRIANGLES, 0, getMinimalBoxVerticesCount());
	}

	void Collider::RenderColliders(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
	{
		if (visible) {
			coloredShader.use();
			coloredShader.setVec3("viewPos", Camera::GetActiveCamera()->Position);
			coloredShader.setMat4("projection", projectionMatrix);
			coloredShader.setMat4("view", viewMatrix);
			for (Collider* col : colliders) {
				col->render();
			}
		}
	}
}