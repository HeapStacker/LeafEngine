#include "RenderableObject.h"
#include <algorithm>
#include "stb_image.h"

static unsigned int SCR_WIDTH;
static unsigned int SCR_HEIGHT;
static unsigned int idAdder = 0;
std::vector<RenderableObject*> RenderableObject::renderableObjects;
static unsigned int lampObjectCount = 0;
static unsigned int flashLightObjectCount = 0;
Shader* multiLightTextureShader = nullptr;
Shader* newColorShader = nullptr;
static bool areLightsSet = false;

struct VerticesData {
	float* vertices;
	unsigned int verticesCount;
	unsigned int VAO;
	unsigned int VBO;
};

struct DiffuseData {
	const char* diffusePath;
	unsigned int diffuseMap;
};

struct SpecularData {
	const char* specularPath;
	unsigned int specularMap;
};

static void DeleteShaders() {
	if (multiLightTextureShader) {
		delete multiLightTextureShader;
	}
	if (newColorShader) {
		delete newColorShader;
	}
	multiLightTextureShader = nullptr;
	newColorShader = nullptr;
}

unsigned int RenderableObject::getID() { return id; }

void RenderableObject::setBoxColider() {
	if (!colisionBundle.colider) {
		colisionBundle.colider = new BoxColider(modelMatrix, id);
		colisionBundle.colisionExecutor = new BoxColisionExecutor((BoxColider*)colisionBundle.colider);
	}
	else {
		std::cerr << "Colider already set!\n";
	}
}

void RenderableObject::setSphereColider() {
	if (!colisionBundle.colider) {
		colisionBundle.colider = new SphereColider(modelMatrix, getID());
		colisionBundle.colisionExecutor = new SphereColisionExecutor((SphereColider*)colisionBundle.colider);
	}
	else {
		std::cerr << "Colider already set!\n";
	}
}

Colider* RenderableObject::getColider() { return colisionBundle.colider; }

void RenderableObject::SetSunDirection(const glm::vec3& direction) {
	if (areLightsSet) {
		multiLightTextureShader->setVec3("dirLight.direction", direction);
		newColorShader->setVec3("dirLight.direction", direction);
	}
}

static void report(int crashId, std::string errorMessage) {
	std::cerr << errorMessage << std::endl;
	glfwTerminate();
	DeleteShaders();
	exit(crashId);
}

void RenderableObject::SetLightProperties(const DirectionalLight& dirLight) {
	if (!areLightsSet) {
		multiLightTextureShader = new Shader("shaders/multiLight.vs", "shaders/multyLight.fs");
		newColorShader = new Shader("shaders/newColorShader.vs", "shaders/newColorShader.fs");
		multiLightTextureShader->use();
		multiLightTextureShader->setInt("numOfPointLights", lampObjectCount);
		multiLightTextureShader->setInt("numOfFlashLights", flashLightObjectCount);
		multiLightTextureShader->setInt("material.texture_diffuse", 0);
		multiLightTextureShader->setInt("material.texture_specular", 1); //create a better solution for more objects and different specular and diffuse maps
		multiLightTextureShader->setFloat("material.shininess", 32.f);
		multiLightTextureShader->setVec3("dirLight.direction", dirLight.direction);
		multiLightTextureShader->setVec3("dirLight.ambient", dirLight.ambient);
		multiLightTextureShader->setVec3("dirLight.diffuse", dirLight.diffuse);
		multiLightTextureShader->setVec3("dirLight.specular", dirLight.specular);
		newColorShader->use();
		newColorShader->setInt("numOfPointLights", lampObjectCount);
		newColorShader->setInt("numOfFlashLights", flashLightObjectCount);
		newColorShader->setVec3("dirLight.direction", dirLight.direction);
		newColorShader->setVec3("dirLight.ambient", dirLight.ambient);
		newColorShader->setVec3("dirLight.diffuse", dirLight.diffuse);
		newColorShader->setVec3("dirLight.specular", dirLight.specular);
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
		std::cerr << "Texture failed to load at path: " << path << std::endl;
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
	static std::string lightId;
	modelMatrix[3] = glm::vec4({ position, 1 });
	if (luminousProperties.flashLampID || luminousProperties.lampID) applyLightToShader();
	if (colisionBundle.colider) {
		colisionBundle.colider->modelMatrix[3] = glm::vec4({ position, 1 });
		ProccesColisions(this);
	}
}

void RenderableObject::translate(const glm::vec3& position) {
	this->setPosition(this->getPosition() + position);
	if (colisionBundle.colider) {
		colisionBundle.colider->translate(position);
		ProccesColisions(this);
	}
}

void RenderableObject::scale(float scalar) {
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scalar));
	if (colisionBundle.colider) {
		colisionBundle.colider->scale(scalar);
		ProccesColisions(this);
	}
}

void RenderableObject::scale(const glm::vec3& scalar) {
	modelMatrix = glm::scale(modelMatrix, scalar);
	if (colisionBundle.colider) {
		colisionBundle.colider->scale(scalar);
		ProccesColisions(this);
	}
}

void RenderableObject::rotateAround(glm::vec3 axis, float degrees) {
	modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), glm::normalize(axis));
	if (colisionBundle.colider) {
		colisionBundle.colider->rotateAround(axis, degrees);
		ProccesColisions(this);
	}
}

glm::vec3 RenderableObject::getPosition() {
	return glm::vec3(modelMatrix[3]);
}

RenderableObject::RenderableObject(std::string modelPath) {
	if (areLightsSet) {
		id = ++idAdder;
		shaderType = 0;
		model = new Model(modelPath);
		RenderableObject::renderableObjects.push_back(this);
	}
	else report(4, "Light properties not set");
}

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
	if (!data || data && data->diffusePath != diffuseTexturePath && lastCreatedMap) {
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
	if (!data || data && data->specularPath != specularTexturePath && lastCreatedMap) {
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
		id = ++idAdder;
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
	else report(4, "Light properties not set");
}

static void SetVaoVboMinimised(float* vertices, unsigned int& verticesSize, unsigned int& vao, unsigned int& vbo) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

static void SetVerticesMinimised(RenderableObject* renderableObject, float* vertices, unsigned int& verticesSize) {
	unsigned int tempVao, tempVbo;
	SetVaoVboMinimised(vertices, verticesSize, tempVao, tempVbo);
	VerticesData tempData = { vertices, verticesSize / sizeof(float), tempVao, tempVbo };
	consumedVertices.push_back(tempData);
	SetMaterialData(renderableObject, tempVao, tempVbo, verticesSize / sizeof(float));
}

RenderableObject::RenderableObject(float* vertices, unsigned int verticesSize, const glm::vec3& color) {
	if (areLightsSet) {
		id = ++idAdder;
		shaderType = 1;
		static bool isFirstTime = true;
		materialData.color = color;
		if (isFirstTime) {
			SetVerticesMinimised(this, vertices, verticesSize);
			isFirstTime = false;
		}
		else AssignVertices(this, vertices, verticesSize);
		RenderableObject::renderableObjects.push_back(this);
	}
	else report(4, "Light properties not set");
}

RenderableObject::~RenderableObject() {
	if (model) delete model;
	if (colisionBundle.colider) delete colisionBundle.colider;
	if (colisionBundle.colisionExecutor) delete colisionBundle.colisionExecutor;
	glDeleteVertexArrays(1, &materialData.VAO);
	glDeleteBuffers(1, &materialData.VBO);
	Erase(id);
	if (renderableObjects.size() == 0) { DeleteShaders(); } //needs to be after Erase(id)! (otherwise it would be a mem leak)
}

void RenderableObject::turnToLamp(const glm::vec3& color, float lightMultiplier, const Attenuation& attenuation) {
	if (!luminousProperties.lampID) {
		luminousProperties.lampID = ++lampObjectCount;
		multiLightTextureShader->use();
		multiLightTextureShader->setInt("numOfPointLights", lampObjectCount);
		newColorShader->use();
		newColorShader->setInt("numOfPointLights", lampObjectCount);
		luminousProperties.color = color;
		luminousProperties.lightMultiplier = lightMultiplier;
		luminousProperties.attenuation = attenuation;
		applyLightToShader();
	}
}

void RenderableObject::turnToSpotLight(const glm::vec3& direction, float innerCutOff, float outerCutOff, const glm::vec3& color, float lightMultiplier, const Attenuation& attenuation) {
	if (!luminousProperties.flashLampID) {
		luminousProperties.flashLampID = ++flashLightObjectCount;
		multiLightTextureShader->use();
		multiLightTextureShader->setInt("numOfFlashLights", flashLightObjectCount);
		newColorShader->use();
		newColorShader->setInt("numOfFlashLights", flashLightObjectCount);
		luminousProperties.direction = direction;
		luminousProperties.innerCutOff = getSpotLightCutOff(innerCutOff);
		luminousProperties.outerCutOff = getSpotLightCutOff(outerCutOff);
		luminousProperties.color = color;
		luminousProperties.lightMultiplier = lightMultiplier;
		luminousProperties.attenuation = attenuation;
		applyLightToShader();
	}
}

void RenderableObject::applyLightToShader() {
	static std::string lightId;
	if (luminousProperties.flashLampID) {
		multiLightTextureShader->use();
		lightId = "flashLights[" + std::to_string(luminousProperties.flashLampID - 1) + "]";
		multiLightTextureShader->setVec3(lightId + ".direction", luminousProperties.direction);
		multiLightTextureShader->setFloat(lightId + ".cutOff", luminousProperties.innerCutOff);
		multiLightTextureShader->setFloat(lightId + ".outerCutOff", luminousProperties.outerCutOff);
		newColorShader->use();
		newColorShader->setVec3(lightId + ".direction", luminousProperties.direction);
		newColorShader->setFloat(lightId + ".cutOff", luminousProperties.innerCutOff);
		newColorShader->setFloat(lightId + ".outerCutOff", luminousProperties.outerCutOff);
	}
	else lightId = "pointLights[" + std::to_string(luminousProperties.lampID - 1) + "]";
	multiLightTextureShader->use();
	multiLightTextureShader->setVec3(lightId + ".position", getPosition());
	multiLightTextureShader->setFloat(lightId + ".constant", luminousProperties.attenuation.constantMultiplier);
	multiLightTextureShader->setFloat(lightId + ".linear", luminousProperties.attenuation.linearMultiplier);
	multiLightTextureShader->setFloat(lightId + ".quadratic", luminousProperties.attenuation.quadraticMultiplier);
	multiLightTextureShader->setVec3(lightId + ".ambient", luminousProperties.color * luminousProperties.lightMultiplier);
	multiLightTextureShader->setVec3(lightId + ".diffuse", luminousProperties.color);
	multiLightTextureShader->setVec3(lightId + ".specular", luminousProperties.color);
	newColorShader->use();
	newColorShader->setVec3(lightId + ".position", getPosition());
	newColorShader->setFloat(lightId + ".constant", luminousProperties.attenuation.constantMultiplier);
	newColorShader->setFloat(lightId + ".linear", luminousProperties.attenuation.linearMultiplier);
	newColorShader->setFloat(lightId + ".quadratic", luminousProperties.attenuation.quadraticMultiplier);
	newColorShader->setVec3(lightId + ".ambient", luminousProperties.color * luminousProperties.lightMultiplier);
	newColorShader->setVec3(lightId + ".diffuse", luminousProperties.color);
	newColorShader->setVec3(lightId + ".specular", luminousProperties.color);
}

void RenderableObject::changeSpotLightDirection(const glm::vec3& direction) {
	luminousProperties.direction = direction;
	applyLightToShader();
}

//object isn't fully removed (only at the end of the program)
void RenderableObject::Erase(unsigned int id) { 
	RenderableObject* object = RenderableObject::FindById(id);
	RenderableObject::renderableObjects.erase(std::remove(RenderableObject::renderableObjects.begin(), RenderableObject::renderableObjects.end(), object), RenderableObject::renderableObjects.end());
}

void setShaderDrawProperties(Shader* shader, Camera* camera, glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
	shader->use();
	shader->setVec3("viewPos", camera->Position);
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setMat4("model", model);
}

void RenderableObject::ProccesColisions(RenderableObject* transformedObject) {
	for (RenderableObject* obj : renderableObjects) {
		if (transformedObject->colisionBundle.colider && obj->colisionBundle.colider && transformedObject->id != obj->id) {
			obj->colisionBundle.colider->acceptColisionExecution(transformedObject->colisionBundle.colisionExecutor);
		}
	}
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
				glBindTexture(GL_TEXTURE_2D, renderableObjects[i]->materialData.specularMap);
				glBindVertexArray(renderableObjects[i]->materialData.VAO);
				glDrawArrays(GL_TRIANGLES, 0, renderableObjects[i]->materialData.verticesCount);
			}
		}
		if (renderableObjects[i]->shaderType == 1) {
			setShaderDrawProperties(newColorShader, camera, renderableObjects[i]->modelMatrix, view, projection);
			newColorShader->setVec3("color", renderableObjects[i]->materialData.color);
			glBindVertexArray(renderableObjects[i]->materialData.VAO);
			glDrawArrays(GL_TRIANGLES, 0, renderableObjects[i]->materialData.verticesCount);
		}
		if (renderableObjects[i]->colisionBundle.colider) {
			renderableObjects[i]->colisionBundle.colider->draw(camera, view, projection);
		}
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}