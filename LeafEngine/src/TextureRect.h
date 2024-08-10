#pragma once
#include <glm/glm.hpp>
#include "TransformableObject.h"

namespace lf {
	class TextureRect : public TransformableObject {
		unsigned int diffuseMap, specularMap;
		bool visible = true, transparent = false;
		void render();
	public:
		static void RenderRects(glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
		static void RenderTransparentRects(glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
		TextureRect(const glm::vec3& position, const char* diffuseTexturePath, const char* specularTexturePath);
		TextureRect(MovableObject* position, const char* diffuseTexturePath, const char* specularTexturePath);
		void setVisibility(bool visible);
		void remove();
	};
}