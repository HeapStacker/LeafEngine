#include "DirectionalLight.h"
#include "Shader.h"

namespace lf {
	static std::vector<DirectionalLight*> directionalLights;
	static Shader& normalShader = Shader::getNormalShader();
	static Shader& coloredShader = Shader::getColoredShader();
	static unsigned int IdCounter = 0;

	void DirectionalLight::update()
	{
		static std::string lightId;
		lightId = "dirLights[" + std::to_string(id) + "]";
		normalShader.use();
		normalShader.setInt("numOfDirLights", IdCounter);
		normalShader.setVec3(lightId + ".direction", this->RotateableObject::getRotation());
		normalShader.setVec3(lightId + ".ambient", this->color * this->ambientMultiplier);
		normalShader.setVec3(lightId + ".diffuse", this->color);
		normalShader.setVec3(lightId + ".specular", this->color);
		coloredShader.use();
		coloredShader.setInt("numOfDirLights", IdCounter);
		coloredShader.setVec3(lightId + ".direction", this->RotateableObject::getRotation());
		coloredShader.setVec3(lightId + ".ambient", this->color * this->ambientMultiplier);
		coloredShader.setVec3(lightId + ".diffuse", this->color);
	}

	DirectionalLight::DirectionalLight(const glm::vec3& color, const glm::vec3& orientation)
	{
		id = IdCounter++;
		Light::changeColor(color);
		RotateableObject::orient(orientation);
		update();
	}
}