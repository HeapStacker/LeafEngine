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

static float getSpotLightCutOff(float degrees) {
	return glm::cos(glm::radians(degrees));
}

struct SpotLight {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	const Attenuation& attenuation;
	float innerCutOff = getSpotLightCutOff(10.f);
	float outerCutOff = getSpotLightCutOff(15.f);
};