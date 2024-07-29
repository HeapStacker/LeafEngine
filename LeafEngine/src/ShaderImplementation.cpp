#include "ShaderImplementation.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Window.h"

namespace lf {
	static Shader normalShader, coloredShader;

	void loadGlad() {
		static bool firstCall = true;
		
	}

	void initializeShaders()
	{
		static bool firstCall = true;
		if (firstCall) {
			normalShader = Shader("shaders/multiLight.vs", "shaders/multyLight.fs");
			coloredShader = Shader("shaders/newColorShader.vs", "shaders/newColorShader.fs");
			normalShader.use();
			normalShader.setFloat("material.shininess", 32.f);
			normalShader.setInt("material.texture_diffuse", 0);
			normalShader.setInt("material.texture_specular", 1);
			normalShader.setInt("numOfDirLights", 0);
			normalShader.setInt("numOfPointLights", 0);
			normalShader.setInt("numOfFlashLights", 0);
			coloredShader.use();
			coloredShader.setInt("numOfDirLights", 0);
			coloredShader.setInt("numOfPointLights", 0);
			coloredShader.setInt("numOfFlashLights", 0);
			firstCall = false;
		}
	}

	void setShaderDrawProperties(Shader* shader, Camera* camera, glm::mat4& view, glm::mat4& projection) {
		shader->use();
		shader->setVec3("viewPos", camera->Position);
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
	}

	Shader& getNormalShader()
	{
		return normalShader;
	}

	Shader& getColoredShader()
	{
		return coloredShader;
	}
}
