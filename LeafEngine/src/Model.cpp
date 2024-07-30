#include "Model.h"
#include "Shader.h"
#include <glad/glad.h>
#include <vector>

#include "Window.h"
#include "Camera.h"

namespace lf {
	struct ModelDictionary {
		std::string modelPath;
		internal::ModelLoader model;
	};

	static Shader& normalShader = Shader::getNormalShader();

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
		normalShader.use();
		normalShader.setVec3("viewPos", Camera::GetActiveCamera()->Position);
		normalShader.setMat4("projection", projectionMatrix);
		normalShader.setMat4("view", viewMatrix);
		for (Model* model : models) {
			if (model->visible) {
				model->render();
			}
		}
	}

	void Model::setVisibility(bool visible)
	{
		this->visible = visible;
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

	void Model::remove()
	{
		models.erase(std::remove(models.begin(), models.end(), this), models.end());
	}
}