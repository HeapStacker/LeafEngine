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
	ColisionBundle colisionBundle;
public:
	MaterialData materialData;
	static unsigned int luminousObjectsCount;
	static void SetLightProperties(const DirectionalLight& dirLight, const SpotLight& spotLight);
	static void SetWindowWidthHeight(const unsigned int& width, const unsigned int& height);
	void setPosition(const glm::vec3& position);
	void translate(const glm::vec3& position);

	void scale(float scalar) {
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scalar));
		if (colisionBundle.colider) ProccesColisions(this);
	}

	void scale(const glm::vec3& scalar) {
		modelMatrix = glm::scale(modelMatrix, scalar);
		if (colisionBundle.colider) ProccesColisions(this);
	}

	void rotateAround(glm::vec3 axis, float degrees) {
		modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), glm::normalize(axis));
		if (colisionBundle.colider) ProccesColisions(this);
	}

	glm::vec3 getPosition() {
		return glm::vec3(modelMatrix[3]);
	}

	RenderableObject() {}
	RenderableObject(std::string modelPath);
	RenderableObject(float* vertices, unsigned int verticesCount, const char* diffuseTexturePath, const char* specularTexturePath);
	RenderableObject(float* vertices, unsigned int verticesCount, const glm::vec3& color);
	~RenderableObject();
	void turnToLamp(const glm::vec3& color, float lightMultiplier, const Attenuation& attenuation);
	unsigned int getShaderType() { return shaderType; }
	static void Erase(unsigned int id);
	static RenderableObject* FindById(unsigned int id);
	static void ReadActiveIDs();
	static void RenderObjects(GLFWwindow* window, Camera* camera);
	static void ProccesColisions(RenderableObject* transformedObject);

	void setBoxColider(const glm::vec3& sides) {
		if (!colisionBundle.colider) {
			colisionBundle.colider = new BoxColider(&modelMatrix, sides);
			colisionBundle.colisionExecutor = new BoxColisionExecutor((BoxColider*)colisionBundle.colider);
		}
		else {
			std::cerr << "Colider already set!\n";
		}
	}

	void setSphereColider(float radius) {
		if (!colisionBundle.colider) {
			colisionBundle.colider = new SphereColider(&modelMatrix, radius);
			colisionBundle.colisionExecutor = new SphereColisionExecutor((SphereColider*)colisionBundle.colider);
		}
		else {
			std::cerr << "Colider already set!\n";
		}
	}
};