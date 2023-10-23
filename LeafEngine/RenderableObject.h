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
#include "BoxColider.h"

struct MaterialData {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int diffuseMap;
	unsigned int specularMap;
	unsigned int verticesCount;
};

//usefull when creating multiple light objects with same settings
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

struct Box {
	glm::mat4* mMatrix = nullptr;  
	glm::vec3 halfExtents; // Pola duljine stranica kvadra
};

bool isCollision(const Box& box1, const Box& box2);

class RenderableObject
{
	unsigned int id;
	Model* model = nullptr;
	static unsigned int IdAdder;
	static std::vector<RenderableObject*> renderableObjects;
	glm::mat4 modelMatrix = glm::mat4(1.f);
	unsigned int shaderType = 0;
	bool isLuminous = false;
	unsigned int luminousObjectId = 0;
	static bool areLightsSet;
	Box* boxColider = nullptr;
	void checkForColisions();
public:
	MaterialData materialData;
	static unsigned int luminousObjectsCount;
	static void SetLightProperties(const DirectionalLight& dirLight, const SpotLight& spotLight);
	static void SetWindowWidthHeight(const unsigned int& width, const unsigned int& height);

	//we use this function also to reset model matrix values
	//example for non constant rotation in game loop...
	//object.setPosition(); --> without this the object will rotate non stop
	//object.rotateAround(glm::normalize(glm::vec3(0.1f, 0.1f, 0.f)), glm::radians(50.f));
	void setPosition(const glm::vec3& position);
	void translate(const glm::vec3& position);

	void scale(float scalar) {
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scalar));
		checkForColisions();
	}

	void rotateAround(glm::vec3 axis, float degrees) {
		modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), glm::normalize(axis));
		checkForColisions();
	}

	RenderableObject() {}
	RenderableObject(std::string modelPath, bool isColideable = false);
	RenderableObject(float* vertices, unsigned int verticesCount, const char* diffuseTexturePath, const char* specularTexturePath);
	RenderableObject(float* vertices, unsigned int verticesCount);
	~RenderableObject();
	void turnToLamp(const glm::vec3& color, float lightMultiplier, const Attenuation& attenuation);
	unsigned int getShaderType() { return shaderType; }
	static void Erase(unsigned int id);
	static RenderableObject* FindById(unsigned int id);
	static void ReadActiveIDs();
	static void RenderObjects(GLFWwindow* window, Camera* camera);

	void setBoxColider(glm::vec3 sides = {1.f, 1.f, 1.f}) { //default box colider is a cube 1.f units each side
		std::cout << "Colider size = " << glm::to_string(sides) << std::endl;
		this->boxColider = new Box;
		boxColider->mMatrix = &modelMatrix;
		boxColider->halfExtents = sides / 2.f;
	}

	void deleteColider() {
		if (boxColider) delete boxColider;
	}

	glm::vec3 getPosition() {
		return glm::vec3(modelMatrix[3]);
	}
};
