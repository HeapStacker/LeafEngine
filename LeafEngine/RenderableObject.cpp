#include "RenderableObject.h"
#include <algorithm>
#include "stb_image.h"

static unsigned int SCR_WIDTH;
static unsigned int SCR_HEIGHT;
unsigned int RenderableObject::IdAdder = 0;
std::vector<RenderableObject*> RenderableObject::renderableObjects;
unsigned int RenderableObject::luminousObjectsCount = 0;
static Shader* multiLightTextureShader = nullptr;
static Shader* colorShader = nullptr;
bool RenderableObject::areLightsSet = false;

static void DeleteShaders() {
	if (multiLightTextureShader) delete multiLightTextureShader;
	if (colorShader) delete colorShader;
	multiLightTextureShader = nullptr;
	colorShader = nullptr;
}

void crashAndReport(int crashId, std::string errorMessage) {
	std::cout << errorMessage << std::endl;
	glfwTerminate();
	DeleteShaders();
	exit(crashId);
}

static glm::vec3 getPos(glm::mat4& mMatrix) {
	return glm::vec3(mMatrix[3]);
}

// Funkcija za provjeru kolizije izmeðu dva kvadra u 3D prostoru
static bool isCollision(const Box& box1, const Box& box2) {
	// Osobe kvadra
	std::vector<glm::vec3> axes = {
		glm::vec3(1, 0, 0), // X os
		glm::vec3(0, 1, 0), // Y os
		glm::vec3(0, 0, 1)  // Z os
	};

	// Razlika vektora pozicija dvaju kvadara
	glm::vec3 delta = getPos(*box2.mMatrix) - getPos(*box1.mMatrix);

	// Provjera kolizije za svaku os
	for (const auto& axis : axes) {
		float projection1 = glm::dot(getPos(*box1.mMatrix), axis);
		float min1 = projection1 - glm::dot(box1.halfExtents, glm::abs(axis));
		float max1 = projection1 + glm::dot(box1.halfExtents, glm::abs(axis));

		float projection2 = glm::dot(getPos(*box2.mMatrix), axis);
		float min2 = projection2 - glm::dot(box2.halfExtents, glm::abs(axis));
		float max2 = projection2 + glm::dot(box2.halfExtents, glm::abs(axis));

		// Provjerite preklapanje na trenutnoj osi
		if (max1 < min2 || max2 < min1) {
			return false; // Nema kolizije na ovoj osi
		}
	}

	// Dodatna provjera za vektore delta
	for (const auto& axis : axes) {
		float deltaProjection = glm::dot(delta, axis);
		float sumExtents = glm::dot(box1.halfExtents, glm::abs(axis)) + glm::dot(box2.halfExtents, glm::abs(axis));

		// Provjerite koliziju na trenutnoj osi
		if (glm::abs(deltaProjection) > sumExtents) {
			return false; // Nema kolizije na ovoj osi
		}
	}

	return true; // Preklapanje na svim osima
}

void RenderableObject::checkForColisions() {
	for (RenderableObject* obj : renderableObjects) {
		if (obj->boxColider && this->id != obj->id) {
			if (isCollision(*this->boxColider, *obj->boxColider)) std::cout << "Another colision\n";
		}
	}
}

void RenderableObject::SetLightProperties(const DirectionalLight& dirLight, const SpotLight& spotLight) {
	if (!areLightsSet) {
		multiLightTextureShader = new Shader("lighting.vs", "multyLight.fs");
		colorShader = new Shader("lightObj.vs", "lightObj.fs");
		multiLightTextureShader->use();
		multiLightTextureShader->setInt("numOfPointLights", RenderableObject::luminousObjectsCount);
		multiLightTextureShader->setInt("material.texture_diffuse", 0);
		multiLightTextureShader->setInt("material.texture_specular", 1); //create a better solution for more objects and different specular and diffuse maps
		multiLightTextureShader->setFloat("material.shininess", 32.f);
		multiLightTextureShader->setVec3("dirLight.direction", dirLight.direction);
		multiLightTextureShader->setVec3("dirLight.ambient", dirLight.ambient);
		multiLightTextureShader->setVec3("dirLight.diffuse", dirLight.diffuse);
		multiLightTextureShader->setVec3("dirLight.specular", dirLight.specular);
		multiLightTextureShader->setVec3("spotLight.ambient", spotLight.ambient);
		multiLightTextureShader->setVec3("spotLight.diffuse", spotLight.diffuse);
		multiLightTextureShader->setVec3("spotLight.specular", spotLight.specular);
		multiLightTextureShader->setFloat("spotLight.constant", spotLight.attenuation.constantMultiplier);
		multiLightTextureShader->setFloat("spotLight.linear", spotLight.attenuation.linearMultiplier);
		multiLightTextureShader->setFloat("spotLight.quadratic", spotLight.attenuation.quadraticMultiplier);
		multiLightTextureShader->setFloat("spotLight.cutOff", spotLight.innerCutOff);
		multiLightTextureShader->setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
		areLightsSet = true;
	}
}

void RenderableObject::SetWindowWidthHeight(const unsigned int& width, const unsigned int& height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

RenderableObject* RenderableObject::FindById(unsigned int id) {
	for (int i = 0; i < RenderableObject::renderableObjects.size(); i++) {
		if (RenderableObject::renderableObjects[i]->id == id) return RenderableObject::renderableObjects[i];
	}
	return nullptr;
}

void RenderableObject::setPosition(const glm::vec3& position) {
	if (isLuminous) {
		std::string lightId = "pointLights[" + std::to_string(luminousObjectId) + "]";
		multiLightTextureShader->setVec3(lightId + ".position", position);
	}
	modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::translate(modelMatrix, position);
	if (boxColider) checkForColisions();
}

void RenderableObject::translate(const glm::vec3& position) {
	if (isLuminous) {
		std::string lightId = "pointLights[" + std::to_string(luminousObjectId) + "]";
		multiLightTextureShader->setVec3(lightId + ".position", position);
	}
	modelMatrix = glm::translate(modelMatrix, getPosition() + position);
	if (boxColider) checkForColisions();
}

RenderableObject::RenderableObject(std::string modelPath, bool isColideable) {
	if (areLightsSet) {
		id = ++RenderableObject::IdAdder;
		shaderType = 0;
		model = new Model(modelPath);
		RenderableObject::renderableObjects.push_back(this);
	}
	else crashAndReport(4, "Light properties not set");
}

struct VerticesData {
	float* vertices;
	unsigned int verticesCount;
	unsigned int VAO;
	unsigned int VBO;
};

//Diffuse flyweight
struct DiffuseData {
	const char* diffusePath;
	unsigned int diffuseMap;
};

//Specular flyweight
struct SpecularData {
	const char* specularPath;
	unsigned int specularMap;
};

static std::vector<VerticesData> consumedVertices;
static std::vector<DiffuseData> consumedDiffuseTextures;
static std::vector<SpecularData> consumedSpecularTextures;

static void SetVaoVbo(float* vertices, unsigned int& verticesSize, unsigned int& vao, unsigned int& vbo) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

static void SetMaterialData(RenderableObject* renderableObject, unsigned int& vao, unsigned int& vbo, unsigned int&& verticesCount) {
	renderableObject->materialData.VAO = vao;
	renderableObject->materialData.VBO = vbo;
	renderableObject->materialData.verticesCount = verticesCount;
}

static bool AssignOrCreateDiffuseMap(RenderableObject* renderableObject, DiffuseData* data, const char* diffuseTexturePath, bool lastCreatedMap = false) {
	if (data && data->diffusePath == diffuseTexturePath) { //validate if it's the same diffuse texture path
		renderableObject->materialData.diffuseMap = data->diffuseMap; //set materials diffuse map
		return true;
	}
	if (!data || data->diffusePath != diffuseTexturePath && lastCreatedMap) {
		DiffuseData tempData = { diffuseTexturePath, loadTexture(diffuseTexturePath) };
		consumedDiffuseTextures.push_back(tempData);
		renderableObject->materialData.diffuseMap = tempData.diffuseMap;
	}
	return false;
}

static bool AssignOrCreateSpecularMap(RenderableObject* renderableObject, SpecularData* data, const char* specularTexturePath, bool lastCreatedMap = false) {
	if (data && data->specularPath == specularTexturePath) { //validate if it's the same specular texture path
		renderableObject->materialData.specularMap = data->specularMap; //set materials specular map
		return true;
	}
	if (!data || data->specularPath != specularTexturePath && lastCreatedMap) {
		SpecularData tempData = { specularTexturePath, loadTexture(specularTexturePath) };
		consumedSpecularTextures.push_back(tempData);
		renderableObject->materialData.specularMap = tempData.specularMap;
	}
	return false;
}

static void SetVertices(RenderableObject* renderableObject, float* vertices, unsigned int& verticesSize) {
	unsigned int tempVao, tempVbo;
	SetVaoVbo(vertices, verticesSize, tempVao, tempVbo);
	VerticesData tempData = { vertices, verticesSize / sizeof(float), tempVao, tempVbo };
	consumedVertices.push_back(tempData);
	SetMaterialData(renderableObject, tempVao, tempVbo, verticesSize / sizeof(float));
}

static void AssignVertices(RenderableObject* renderableObject, float* vertices, unsigned int& verticesSize) {
	bool areVerticesAssigned = false;
	for (int i = 0; i < consumedVertices.size(); i++) {
		if (consumedVertices[i].verticesCount == verticesSize / sizeof(float)) {
			for (int j = 0; j < verticesSize / sizeof(float); j++) {
				if (consumedVertices[i].vertices[j] != vertices[j]) break;
				else {
					if (j == verticesSize / sizeof(float) - 1) {
						SetMaterialData(renderableObject, consumedVertices[i].VAO, consumedVertices[i].VBO, verticesSize / sizeof(float));
						areVerticesAssigned = true;
					}
				}
			}
			if (areVerticesAssigned) break;
		}
	}
	if (!areVerticesAssigned) {
		SetVertices(renderableObject, vertices, verticesSize);
	}
}

RenderableObject::RenderableObject(float* vertices, unsigned int verticesSize, const char* diffuseTexturePath, const char* specularTexturePath) {
	if (areLightsSet) {
		id = ++RenderableObject::IdAdder;
		shaderType = 0;
		static bool isFirstTime = true;
		if (isFirstTime) {
			AssignOrCreateDiffuseMap(this, nullptr, diffuseTexturePath, true);
			AssignOrCreateSpecularMap(this, nullptr, specularTexturePath, true);
			SetVertices(this, vertices, verticesSize);
			isFirstTime = false;
		}
		else {
			//this part of code checks if diffuse and specular textures alreay exist (set if exist, create and set if it doesn't exist)
			for (int i = 0; i < consumedDiffuseTextures.size(); i++) {
				if (AssignOrCreateDiffuseMap(this, &consumedDiffuseTextures[i], diffuseTexturePath, i + 1 == consumedDiffuseTextures.size())) break;
			}
			for (int i = 0; i < consumedSpecularTextures.size(); i++) {
				if (AssignOrCreateSpecularMap(this, &consumedSpecularTextures[i], specularTexturePath, i + 1 == consumedSpecularTextures.size())) break;
			}
			AssignVertices(this, vertices, verticesSize);
		}
		RenderableObject::renderableObjects.push_back(this);
	} 
	else crashAndReport(4, "Light properties not set");
}

static void SetVaoVboMinimised(float* vertices, unsigned int& verticesSize, unsigned int& vao, unsigned int& vbo) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

static void SetVerticesMinimised(RenderableObject* renderableObject, float* vertices, unsigned int& verticesSize) {
	unsigned int tempVao, tempVbo;
	SetVaoVboMinimised(vertices, verticesSize, tempVao, tempVbo);
	VerticesData tempData = { vertices, verticesSize / sizeof(float), tempVao, tempVbo };
	consumedVertices.push_back(tempData);
	SetMaterialData(renderableObject, tempVao, tempVbo, verticesSize / sizeof(float));
}

RenderableObject::RenderableObject(float* vertices, unsigned int verticesSize) {
	if (areLightsSet) {
		id = ++RenderableObject::IdAdder;
		shaderType = 1;
		static bool isFirstTime = true;
		if (isFirstTime) {
			SetVerticesMinimised(this, vertices, verticesSize);
			isFirstTime = false;
		}
		else AssignVertices(this, vertices, verticesSize);
		RenderableObject::renderableObjects.push_back(this);
	}
	else crashAndReport(4, "Light properties not set");
}

RenderableObject::~RenderableObject() {
	if (model) delete model;
	glDeleteVertexArrays(1, &materialData.VAO);
	glDeleteBuffers(1, &materialData.VBO);
	if (renderableObjects.size() == 0) { DeleteShaders(); } //this would lead to a major bug if we wanted to run empty content without any objects
	deleteColider();
	Erase(id);
}

void RenderableObject::turnToLamp(const glm::vec3& color, float lightMultiplier, const Attenuation& attenuation) {
	isLuminous = true;
	luminousObjectId = luminousObjectsCount;
	luminousObjectsCount++;
	std::string lightId = "pointLights[" + std::to_string(luminousObjectId) + "]";
	multiLightTextureShader->use();
	multiLightTextureShader->setInt("numOfPointLights", luminousObjectsCount);
	multiLightTextureShader->setVec3(lightId + ".position", glm::vec3(modelMatrix[3]));
	multiLightTextureShader->setVec3(lightId + ".ambient", color * lightMultiplier);
	multiLightTextureShader->setVec3(lightId + ".diffuse", color);
	multiLightTextureShader->setVec3(lightId + ".specular", color);
	multiLightTextureShader->setFloat(lightId + ".constant", attenuation.constantMultiplier);
	multiLightTextureShader->setInt(lightId + ".linear", attenuation.linearMultiplier);
	multiLightTextureShader->setInt(lightId + ".quadratic", attenuation.quadraticMultiplier);
}

//object isn't fully removed (only at the end of the program)
void RenderableObject::Erase(unsigned int id) { 
	RenderableObject* object = RenderableObject::FindById(id);
	RenderableObject::renderableObjects.erase(std::remove(RenderableObject::renderableObjects.begin(), RenderableObject::renderableObjects.end(), object), RenderableObject::renderableObjects.end());
}

void RenderableObject::ReadActiveIDs() {
	for (int i = 0; i < RenderableObject::renderableObjects.size(); i++) {
		std::cout << RenderableObject::renderableObjects[i]->id << ". id is active\n";
	}
}

void setShaderDrawProperties(Shader* shader, Camera* camera, glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
	shader->use();
	shader->setVec3("viewPos", camera->Position);
	shader->setVec3("spotLight.position", camera->Position);
	shader->setVec3("spotLight.direction", camera->Front);
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setMat4("model", model);
}

void RenderableObject::RenderObjects(GLFWwindow* window, Camera* camera) {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //make that the clear color can be changed somewhere
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //change the way of passing SCR_WIDTH & SCR_HEIGHT
	glm::mat4 view = camera->GetViewMatrix();
	for (int i = 0; i < RenderableObject::renderableObjects.size(); i++) {
		if (renderableObjects[i]->shaderType == 0) {
			setShaderDrawProperties(multiLightTextureShader, camera, renderableObjects[i]->modelMatrix, view, projection);
			if (renderableObjects[i]->model) {
				renderableObjects[i]->model->Draw(*multiLightTextureShader);
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, renderableObjects[i]->materialData.diffuseMap);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, renderableObjects[i]->materialData.diffuseMap);
				glBindVertexArray(renderableObjects[i]->materialData.VAO);
				glDrawArrays(GL_TRIANGLES, 0, renderableObjects[i]->materialData.verticesCount);
			}
		}
		if (renderableObjects[i]->shaderType == 1) {
			setShaderDrawProperties(colorShader, camera, renderableObjects[i]->modelMatrix, view, projection);
			glBindVertexArray(renderableObjects[i]->materialData.VAO);
			glDrawArrays(GL_TRIANGLES, 0, renderableObjects[i]->materialData.verticesCount);
		}
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}
