#include "lightProperties.h"
#include "RenderableObject.h"

float getSpotLightCutOff(float degrees) {
	return glm::cos(glm::radians(degrees));
}

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

void setLighting(const glm::vec3& sunDirection, AmbientMode mode)
{
	DirectionalLight dirLight;
	//should be done with polymorphism
	dirLight.direction = sunDirection;
	switch (mode)
	{
	case DESERT:
		dirLight.ambient = { 0.3f, 0.24f, 0.14f };
		dirLight.diffuse = { 0.7f, 0.42f, 0.26f };
		dirLight.specular = { 0.5f, 0.5f, 0.5f };
		break;
	case FACTORY:
		dirLight.ambient = { 0.05f, 0.05f, 0.1f };
		dirLight.diffuse = { 0.2f, 0.2f, 0.7 };
		dirLight.specular = { 0.7f, 0.7f, 0.7f };
		break;
	case HORROR:
		dirLight.ambient = { 0.0f, 0.0f, 0.0f };
		dirLight.diffuse = { 0.05f, 0.05f, 0.05 };
		dirLight.specular = { 0.2f, 0.2f, 0.2f };
		break;
	case BIOCHEMICAL_LAB:
		dirLight.ambient = { 0.5f, 0.5f, 0.5f };
		dirLight.diffuse = { 1.0f, 1.0f, 1.0f };
		dirLight.specular = { 0.4f, 0.7f, 0.1f };
		break;
	case NORMAL:
		dirLight.ambient = { 0.5f, 0.5f, 0.5f };
		dirLight.diffuse = { 0.5f, 0.5f, 0.5f };
		dirLight.specular = { 0.f, 0.f, 0.f };
		break;
	default:
		break;
	}
	RenderableObject::SetLightProperties(dirLight);
}