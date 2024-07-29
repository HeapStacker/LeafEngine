#include "PointLight.h"
#include <vector>

namespace lf {
	static std::vector<PointLight*> pointLights;
	static Shader& normalShader = getNormalShader();
	static Shader& coloredShader = getColoredShader();

	void PointLight::changeColor(glm::vec3 color)
	{
		this->color = color;
	}

	void PointLight::changeAmbientMultiplier(float ambientMultiplier)
	{
		this->ambientMultiplier = ambientMultiplier;
	}

	void PointLight::UpdatePointLights() {
		normalShader.use();
		normalShader.setInt("numOfPointLights", pointLights.size());
		coloredShader.use();
		coloredShader.setInt("numOfPointLights", pointLights.size());
		static std::string lightId;
		for (int i = 0; i < pointLights.size(); i++) {
			lightId = "pointLights[" + std::to_string(i) + "]";
			normalShader.use();
			normalShader.setVec3(lightId + ".position", pointLights[i]->getPosition());
			changeShaderLightAttenuation(lightId, normalShader, pointLights[i]->attenuation);
			normalShader.setVec3(lightId + ".ambient", pointLights[i]->color * pointLights[i]->ambientMultiplier);
			normalShader.setVec3(lightId + ".diffuse", pointLights[i]->color);
			normalShader.setVec3(lightId + ".specular", pointLights[i]->color);

			coloredShader.use();
			coloredShader.setVec3(lightId + ".position", pointLights[i]->getPosition());
			changeShaderLightAttenuation(lightId, coloredShader, pointLights[i]->attenuation);
			coloredShader.setVec3(lightId + ".ambient", pointLights[i]->color * pointLights[i]->ambientMultiplier);
			coloredShader.setVec3(lightId + ".diffuse", pointLights[i]->color);
		}
	}

	PointLight::PointLight(glm::vec3 position, glm::vec3 color, Attenuation attenuation, float ambientMultiplier)
	{
		selectLightType(0);
		setPosition(position);
		changeColor(color);
		changeAttenuation(attenuation);
		changeAmbientMultiplier(ambientMultiplier);
		pointLights.push_back(this);
		UpdatePointLights();
	}
}
