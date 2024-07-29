#pragma once
#include "Shader.h"
#include "Camera.h"

namespace lf {
	void loadGlad();

	void initializeShaders();

	void setShaderDrawProperties(Shader* shader, Camera* camera, glm::mat4& view, glm::mat4& projection);

	Shader& getNormalShader();
	Shader& getColoredShader();
}