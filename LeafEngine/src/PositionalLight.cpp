#include "PositionalLight.h"

namespace lf {

	//with settings below...
	//keep in mind, that lights almost fade out at around 20% of said space
	Attenuation attenuations[] = {
		{1.0f, 0.7f, 1.8f},		 //spans 7 units
		{1.0f, 0.35f, 0.44f},		 //spans 13 units
		{1.0f, 0.22f, 0.20f},		 //spans 20 units
		{1.0f, 0.14f, 0.07f},		 //spans 32 units
		{1.0f, 0.09f, 0.032f},		 //spans 50 units
		{1.0f, 0.07f, 0.017f},		 //spans 65 units
		{1.0f, 0.045f, 0.0075f},	 //spans 100 units
		{1.0f, 0.027f, 0.0028f},	 //spans 160 units
		{1.0f, 0.022f, 0.0019f},	 //spans 200 units
		{1.0f, 0.014f, 0.0007f},	 //spans 325 units
		{1.0f, 0.007f, 0.0002f},	 //spans 600 units
		{1.0f, 0.0014f, 0.000007f}  //spans 3250 units
	};

	Attenuation getAttenuation(unsigned int lightPower)
	{
		if (lightPower >= 0 && lightPower <= 11) return attenuations[lightPower];
		else return attenuations[11];
	}

	void changeShaderLightAttenuation(std::string& lightId, Shader& shaderInUse, Attenuation& attenuation)
	{
		shaderInUse.setFloat(lightId + ".constant", attenuation.constantMultiplier);
		shaderInUse.setFloat(lightId + ".linear", attenuation.linearMultiplier);
		shaderInUse.setFloat(lightId + ".quadratic", attenuation.quadraticMultiplier);
	}

	void PositionalLight::changeAttenuation(const Attenuation& attenuation)
	{
		this->attenuation = attenuation;
	}
}