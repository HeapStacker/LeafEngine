#pragma once
#include <glm/glm.hpp>
#include "IdentifiableObject.h"
#include "TransformableObject.h"

namespace lf {
	class ColoredBox : public TransformableObject, public IdentifiableObject {
		void setNewId() override;
		glm::vec3 color = { 1.f, 1.f, 1.f };
		bool visible = true;
		void render();
	public:
		static void RenderColoredBoxes(glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
		ColoredBox(const glm::vec3& color);
		void setVisibility(bool visible);
	};

}