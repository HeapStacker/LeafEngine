#include "SpotLight.h"
#include <vector>

namespace lf {
	static unsigned int spotLightCount = 0;
	static std::vector<SpotLight*> spotLights;
	static Shader& normalShader = Shader::getNormalShader();
	static Shader& coloredShader = Shader::getColoredShader();

	void SpotLight::UpdateSpotLights() {
		normalShader.use();
		normalShader.setInt("numOfFlashLights", spotLightCount);
		coloredShader.use();
		coloredShader.setInt("numOfFlashLights", spotLightCount);

		static std::string lightId;
		for (int i = 0; i < spotLightCount + 1; i++) {
			lightId = "flashLights[" + std::to_string(i) + "]";
			normalShader.use();
			normalShader.setVec3(lightId + ".direction", spotLights[i]->direction);
			normalShader.setFloat(lightId + ".cutOff", spotLights[i]->innerCutOff);
			normalShader.setFloat(lightId + ".outerCutOff", spotLights[i]->outerCutOff);
			coloredShader.use();
			coloredShader.setVec3(lightId + ".direction", spotLights[i]->direction);
			coloredShader.setFloat(lightId + ".cutOff", spotLights[i]->innerCutOff);
			coloredShader.setFloat(lightId + ".outerCutOff", spotLights[i]->outerCutOff);


			normalShader.use();
			normalShader.setVec3(lightId + ".position", spotLights[i]->getPosition());
			normalShader.setFloat(lightId + ".constant", spotLights[i]->attenuation.constantMultiplier);
			normalShader.setFloat(lightId + ".linear", spotLights[i]->attenuation.linearMultiplier);
			normalShader.setFloat(lightId + ".quadratic", spotLights[i]->attenuation.quadraticMultiplier);
			normalShader.setVec3(lightId + ".ambient", spotLights[i]->color * spotLights[i]->ambientMultiplier);
			normalShader.setVec3(lightId + ".diffuse", spotLights[i]->color);
			normalShader.setVec3(lightId + ".specular", spotLights[i]->color);

			coloredShader.use();
			coloredShader.setVec3(lightId + ".position", spotLights[i]->getPosition());
			coloredShader.setFloat(lightId + ".constant", spotLights[i]->attenuation.constantMultiplier);
			coloredShader.setFloat(lightId + ".linear", spotLights[i]->attenuation.linearMultiplier);
			coloredShader.setFloat(lightId + ".quadratic", spotLights[i]->attenuation.quadraticMultiplier);
			coloredShader.setVec3(lightId + ".ambient", spotLights[i]->color * spotLights[i]->ambientMultiplier);
			coloredShader.setVec3(lightId + ".diffuse", spotLights[i]->color);
		}
	}

	SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float innerCutOff, float outerCutOff, Attenuation attenuation, float ambientMultiplier)
	{
		spotLightCount++;
		changeColor(color);
		setPosition(position);
		this->direction = direction;
		changeAmbientMultiplier(ambientMultiplier);
		UpdateSpotLights();
		spotLights.push_back(this);
	}

	void SpotLight::changeColor(glm::vec3 color)
	{
		this->color = color;
	}

	void SpotLight::changeInnerCutOff(float innerCutOff)
	{
		this->innerCutOff = innerCutOff;
	}

	void SpotLight::changeOuterCutOff(float outerCutOff)
	{
		this->outerCutOff = outerCutOff;
	}

	void SpotLight::changeAmbientMultiplier(float ambientMultiplier)
	{
		this->ambientMultiplier = ambientMultiplier;
	}
}
