#pragma once
#include "Camera.h"
#include "model.h"
#include "shader.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include "lightProperties.h"
#include "Colider.h"

struct MaterialData {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int diffuseMap;
	unsigned int specularMap;
	unsigned int verticesCount;
	glm::vec3 color;
};

class RenderableObject;

struct ColisionBundle {
	Colider* colider = nullptr;
	ColisionExecutor* colisionExecutor = nullptr;
	RenderableObject* coliderRepresentation = nullptr;
};

struct LuminousObjectProperties {
	unsigned int lampID = 0;
	unsigned int flashLampID = 0;
	glm::vec3 direction = { 0, 0, 0 };
	float innerCutOff = 0;
	float outerCutOff = 0;
	glm::vec3 color = { 0, 0, 0 };
	float lightMultiplier = 0.f;
	Attenuation attenuation = getAttenuation(0);
};

class RenderableObject
{
	unsigned int id;
	Model* model = nullptr;
	static std::vector<RenderableObject*> renderableObjects;
	glm::mat4 modelMatrix = glm::mat4(1.f);
	LuminousObjectProperties luminousProperties;
	ColisionBundle colisionBundle;
	unsigned int shaderType = 0;
	void applyLightToShader();
public:
	MaterialData materialData;
	static void SetLightProperties(const DirectionalLight& dirLight);
	static void SetWindowWidthHeight(const unsigned int& width, const unsigned int& height);
	void setPosition(const glm::vec3& position);
	void translate(const glm::vec3& position);
	void scale(float scalar);
	void scale(const glm::vec3& scalar);
	void rotateAround(glm::vec3 axis, float degrees);
	glm::vec3 getPosition();
	RenderableObject() {}
	RenderableObject(std::string modelPath);
	RenderableObject(float* vertices, unsigned int verticesCount, const char* diffuseTexturePath, const char* specularTexturePath);
	RenderableObject(float* vertices, unsigned int verticesCount, const glm::vec3& color);
	~RenderableObject();
	void turnToLamp(const glm::vec3& color, float lightMultiplier = 1.f, const Attenuation& attenuation = getAttenuation(4));
	void turnToSpotLight(const glm::vec3& direction, float innerCutOff, float outerCutOff, const glm::vec3& color = {1.f, 1.f, 1.f}, float lightMultiplier = 1.f, const Attenuation& attenuation = getAttenuation(4));
	void changeSpotLightDirection(const glm::vec3& direction);
	static void Erase(unsigned int id);
	static RenderableObject* FindById(unsigned int id);
	static void RenderObjects(GLFWwindow* window, Camera* camera);
	static void ProccesColisions(RenderableObject* transformedObject);
	static void SetSunDirection(const glm::vec3& direction);
	void setSphereColider();
	void setBoxColider();
	unsigned int getID();
	Colider* getColider();
};