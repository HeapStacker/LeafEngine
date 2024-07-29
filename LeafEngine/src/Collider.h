#pragma once
#include "TransformableObject.h"
#include "IdentifiableObject.h"
#include "DllExporter.h"
#include "Camera.h"

namespace lf {
	struct ColisionPair {
		unsigned int id1, id2;
	};

	using ColisionFunction = void(*)(ColisionPair& pair);

	//remember (For other colliders than box colliders, you will need to implement a visitor pattern)
	class Collider : public TransformableObject, public IdentifiableObject {
		static bool isColisionRegistered(Collider* col1, Collider* col2);
		void render();
	protected:
		void setNewId() override;
		bool colidesWith(Collider* colider);
		bool operator==(const Collider& colider);
		bool operator!=(const Collider& colider);
	public:
		glm::vec3 halfExtents;
		Collider();
		void scale(float scalar);
		void scale(const glm::vec3& scalar);
		static void CheckForCollisions();
		static void ResolveCollisions();
		static void AssignCollisionRules(ColisionFunction collisionRuleFunction);
		static void SetVisibility(bool visibility);
		static void RenderColliders(glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
	};
}