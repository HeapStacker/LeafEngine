#pragma once
#include <glm/glm.hpp>
#include "IdentifiableObject.h"
#include "TransformableObject.h"

namespace lf {
	class TexturedBox : public TransformableObject, public IdentifiableObject {
		void setNewId() override;
		unsigned int diffuseMap, specularMap;
		bool visible = true;
		bool outline = false;
		void render();
	public:
		static void RenderTexturedBoxes(glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
		TexturedBox(const glm::vec3& position, const char* diffuseTexturePath, const char* specularTexturePath);
		void setVisibility(bool visible);
		void setOutline(bool outline);
		void remove();
	};
}