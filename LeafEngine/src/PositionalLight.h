#pragma once
#include "Shader.h"
#include "MoveableObject.h"
#include "ShaderImplementation.h"

namespace lf {
	struct Attenuation {
		float constantMultiplier;
		float linearMultiplier;
		float quadraticMultiplier;
	};

	//lightPower 0-11 (0 and 11 included in range)
	Attenuation getAttenuation(unsigned int lightPower);

	void changeShaderLightAttenuation(std::string& lightId, Shader& shaderInUse, Attenuation& attenuation);

	class PositionalLight: public MoveableObject {
	protected:
		bool on = true;
		MoveableObject* linkedObject = nullptr;
		Attenuation attenuation;
		glm::mat4 modelMatrix = glm::mat4(1.f);
		void selectLightType(unsigned int&& type) {
			if (type == 0) {
				isPointLight = true;
			}
			else {
				isSpotLight = true;
			}
		}
	public:
		PositionalLight() {
			linkedModelMatrixM = &this->modelMatrix;
		}
		void changeAttenuation(const Attenuation& attenuation);
		void turnOn() { on = true; }
		void turnOff() { on = false; }
	};
}