#include "Model.h"
#include "ShaderImplementation.h"
#include <glad/glad.h>
#include <vector>

#include "Window.h"
#include "Camera.h"

namespace lf {
	struct ModelDictionary {
		std::string modelPath;
		internal::ModelLoader model;
	};

	static Shader& normalShader = getNormalShader();

	static unsigned int ModelId = 0;
	static std::vector<ModelDictionary> modelDictionary;
	static std::vector<Model*> models;

	void Model::setNewId()
	{
		id = ModelId++;
	}

	void Model::render()
	{
		normalShader.setMat4("model", modelMatrix);
		model->Draw(normalShader);
	}

	void Model::RenderModels(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
	{
		setShaderDrawProperties(&normalShader, Camera::GetActiveCamera(), viewMatrix, projectionMatrix);
		for (Model* model : models) model->render();
	}

	Model::Model(std::string modelPath)
	{
		setNewId();
		for (ModelDictionary& model : modelDictionary) {
			if (model.modelPath == modelPath) {
				this->model = &model.model;
			}
		}
		if (!this->model) {
			modelDictionary.push_back({ modelPath, internal::ModelLoader(modelPath) });
			this->model = &modelDictionary.back().model;
		}
		models.push_back(this);
	}
}