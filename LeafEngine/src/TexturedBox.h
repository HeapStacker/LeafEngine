#pragma once
#include <glm/glm.hpp>
#include "IdentifiableObject.h"
#include "TransformableObject.h"

namespace lf {
	class TexturedBox : public TransformableObject, public IdentifiableObject {
		void setNewId() override;
		unsigned int diffuseMap, specularMap;
		void render();
	public:
		static void RenderTexturedBoxes(glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
		TexturedBox(const char* diffuseTexturePath, const char* specularTexturePath);
	};
}