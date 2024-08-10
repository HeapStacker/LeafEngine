#pragma once
#include <string>
#include <glm/glm.hpp>
#include "ModelLoader.h"
#include "TransformableObject.h"

namespace lf {
	class Model : public TransformableObject {
		void render();
		bool visible = true;
		internal::ModelLoader* model = nullptr;
	public:
		static void RenderModels(glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
		void setVisibility(bool visible);
		Model(const glm::vec3& position, std::string modelPath);
		Model(MovableObject* position, std::string modelPath);
		void remove();
	};
}