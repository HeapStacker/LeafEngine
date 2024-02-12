#pragma once
#include <glm/glm.hpp>

struct Attenuation {
	float constantMultiplier;
	float linearMultiplier;
	float quadraticMultiplier;
};

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

float getSpotLightCutOff(float degrees);

struct SpotLight {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	Attenuation attenuation;
	float innerCutOff = getSpotLightCutOff(10.f);
	float outerCutOff = getSpotLightCutOff(15.f);
};

enum AmbientMode {
	DESERT,
	FACTORY,
	HORROR,
	BIOCHEMICAL_LAB,
	NORMAL
};

//unput is a number 0-11
Attenuation getAttenuation(unsigned int lightPower);

//or just call RenderableObject::SetLightProperties(dirLight) with custom directional light
void setLighting(const glm::vec3& worldLightDirection, AmbientMode mode);