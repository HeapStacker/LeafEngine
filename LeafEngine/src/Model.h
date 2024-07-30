#pragma once
#include <string>
#include <glm/glm.hpp>
#include "ModelLoader.h"
#include "IdentifiableObject.h"
#include "TransformableObject.h"

namespace lf {
	class Model : public TransformableObject, public IdentifiableObject {
		void render();
		bool visible = true;
		void setNewId() override;
		internal::ModelLoader* model = nullptr;
	public:
		static void RenderModels(glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
		void setVisibility(bool visible);
		Model(std::string modelPath);
		void remove();
	};
}