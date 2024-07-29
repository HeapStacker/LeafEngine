#pragma once
#include <string>
#include <glm/glm.hpp>
#include "ModelLoader.h"
#include "IdentifiableObject.h"
#include "TransformableObject.h"

namespace lf {
	class Model : public TransformableObject, public IdentifiableObject {
		void setNewId() override;
		internal::ModelLoader* model = nullptr;
		void render();
	public:
		static void RenderModels(glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
		Model(std::string modelPath);
	};
}