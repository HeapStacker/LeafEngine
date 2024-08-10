#include "SpotLight.h"
#include "Shader.h"
#include <vector>
#include <string>

namespace lf {
	static unsigned int IdCounter = 0;
	static Shader& normalShader = Shader::getNormalShader();
	static Shader& coloredShader = Shader::getColoredShader();

	void SpotLight::update() {
		static std::string lightId;
		lightId = "flashLights[" + std::to_string(id) + "]";
		normalShader.use();
		normalShader.setInt("numOfFlashLights", IdCounter);
		normalShader.setVec3(lightId + ".direction", RotateableObject::getRotation());
		normalShader.setFloat(lightId + ".cutOff", this->innerCutOff);
		normalShader.setFloat(lightId + ".outerCutOff", this->outerCutOff);
		normalShader.setVec3(lightId + ".position", this->getPosition());
		normalShader.setFloat(lightId + ".constant", this->attenuation.constantMultiplier);
		normalShader.setFloat(lightId + ".linear", this->attenuation.linearMultiplier);
		normalShader.setFloat(lightId + ".quadratic", this->attenuation.quadraticMultiplier);
		normalShader.setVec3(lightId + ".ambient", this->Light::color * this->Light::ambientMultiplier);
		normalShader.setVec3(lightId + ".diffuse", this->Light::color);
		normalShader.setVec3(lightId + ".specular", this->Light::color);
		coloredShader.use();
		coloredShader.setInt("numOfFlashLights", IdCounter);
		coloredShader.setVec3(lightId + ".direction", RotateableObject::getRotation());
		coloredShader.setFloat(lightId + ".cutOff", this->innerCutOff);
		coloredShader.setFloat(lightId + ".outerCutOff", this->outerCutOff);
		coloredShader.setVec3(lightId + ".position", this->getPosition());
		coloredShader.setFloat(lightId + ".constant", this->attenuation.constantMultiplier);
		coloredShader.setFloat(lightId + ".linear", this->attenuation.linearMultiplier);
		coloredShader.setFloat(lightId + ".quadratic", this->attenuation.quadraticMultiplier);
		coloredShader.setVec3(lightId + ".ambient", this->Light::color * this->Light::ambientMultiplier);
		coloredShader.setVec3(lightId + ".diffuse", this->Light::color);
	}

	SpotLight::SpotLight(glm::vec3 position, glm::vec3 orientation, glm::vec3 color, float innerCutOff, float outerCutOff, unsigned int intensity)
	{
		id = IdCounter++;
		setPosition(position);
		RotateableObject::orient(orientation);
		changeColor(color);
		this->innerCutOff = innerCutOff;
		this->outerCutOff = outerCutOff;
		changeIntensity(intensity);
		update();
	}

	SpotLight::SpotLight(MovableObject* position, glm::vec3 orientation, glm::vec3 color, float innerCutOff, float outerCutOff, unsigned int intensity)
	{
		id = IdCounter++;
		setPosition(position);
		RotateableObject::orient(orientation);
		changeColor(color);
		this->innerCutOff = innerCutOff;
		this->outerCutOff = outerCutOff;
		changeIntensity(intensity);
		update();
	}
}
