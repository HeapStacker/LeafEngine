#include "PointLight.h"
#include "Shader.h"
#include <vector>

namespace lf {
	static Shader& normalShader = Shader::getNormalShader();
	static Shader& coloredShader = Shader::getColoredShader();
	static unsigned int IdCounter = 0;

	PointLight::PointLight(glm::vec3 position, glm::vec3 color, unsigned int intensity)
	{
		id = IdCounter++;
		Light::changeColor(color);
		MovableObject::setPosition(position);
		changeIntensity(intensity);
		update();
	}

	void PointLight::update()
	{
		static std::string lightId;
		lightId = "pointLights[" + std::to_string(id) + "]";
		normalShader.use();
		normalShader.setInt("numOfPointLights", IdCounter);
		normalShader.setVec3(lightId + ".position", this->MovableObject::getPosition());
		normalShader.setVec3(lightId + ".ambient", this->color * this->ambientMultiplier);
		normalShader.setVec3(lightId + ".diffuse", this->color);
		normalShader.setVec3(lightId + ".specular", this->color);
		normalShader.setFloat(lightId + ".constant", attenuation.constantMultiplier);
		normalShader.setFloat(lightId + ".linear", attenuation.linearMultiplier);
		normalShader.setFloat(lightId + ".quadratic", attenuation.quadraticMultiplier);
		coloredShader.use();
		coloredShader.setInt("numOfPointLights", IdCounter);
		coloredShader.setVec3(lightId + ".position", this->MovableObject::getPosition());
		coloredShader.setVec3(lightId + ".ambient", this->color * this->ambientMultiplier);
		coloredShader.setVec3(lightId + ".diffuse", this->color);
		coloredShader.setFloat(lightId + ".constant", attenuation.constantMultiplier);
		coloredShader.setFloat(lightId + ".linear", attenuation.linearMultiplier);
		coloredShader.setFloat(lightId + ".quadratic", attenuation.quadraticMultiplier);
	}
}
