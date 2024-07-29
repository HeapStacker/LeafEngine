#include "DirectionalLight.h"
#include "ShaderImplementation.h"

namespace lf {
	static std::vector<DirectionalLight*> directionalLights;
	static Shader& normalShader = getNormalShader();
	static Shader& coloredShader = getColoredShader();

	void DirectionalLight::UpdateDirectionalLights()
	{
		normalShader.use();
		normalShader.setInt("numOfDirLights", directionalLights.size());
		coloredShader.use();
		coloredShader.setInt("numOfDirLights", directionalLights.size());
		static std::string lightId;
		for (int i = 0; i < directionalLights.size(); i++) {
			lightId = "dirLights[" + std::to_string(i) + "]";
			normalShader.use();
			normalShader.setVec3(lightId + ".direction", directionalLights[i]->direction);
			normalShader.setVec3(lightId + ".ambient", directionalLights[i]->color * directionalLights[i]->ambientMultiplier);
			normalShader.setVec3(lightId + ".diffuse", directionalLights[i]->color);
			normalShader.setVec3(lightId + ".specular", directionalLights[i]->color);

			coloredShader.use();
			coloredShader.setVec3(lightId + ".direction", directionalLights[i]->direction);
			coloredShader.setVec3(lightId + ".ambient", directionalLights[i]->color * directionalLights[i]->ambientMultiplier);
			coloredShader.setVec3(lightId + ".diffuse", directionalLights[i]->color);
		}
	}

	DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction)
	{
		this->direction = direction;
		this->color = color;
		directionalLights.push_back(this);
		UpdateDirectionalLights();
	}
}