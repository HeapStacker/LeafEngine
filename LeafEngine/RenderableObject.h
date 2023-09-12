#pragma once
#include "Camera.h"
#include "model.h"
#include <vector>

class RenderableObject
{
	unsigned int id;
	Shader shader;
	Model* model = nullptr;
	Camera* camera = nullptr;
	static unsigned int IdAdder;
	static std::vector<RenderableObject*> renderableObjects;
public:
	RenderableObject(Shader shader, std::string modelPath, Camera* camera = nullptr) 
		:shader(shader)
	{
		id = ++RenderableObject::IdAdder;
		model = new Model(modelPath);
		RenderableObject::renderableObjects.push_back(this);
		std::cout << "Object created\n";
	}
	~RenderableObject() {
		delete model;
		Erase(id);
		std::cout << "Object deleted\n";
	}
	static void Erase(unsigned int id);
	static RenderableObject* FindById(unsigned int id);
	static void ReadActiveIDs();
	static void Render();
};

unsigned int RenderableObject::IdAdder = 0;
std::vector<RenderableObject*> RenderableObject::renderableObjects;

RenderableObject* RenderableObject::FindById(unsigned int id) {
	for (int i = 0; i < RenderableObject::renderableObjects.size(); i++) {
		if (RenderableObject::renderableObjects[i]->id == id) return RenderableObject::renderableObjects[i];
	}
	return nullptr;
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

void RenderableObject::Render() {
	for (int i = 0; i < RenderableObject::renderableObjects.size(); i++) {
		RenderableObject::renderableObjects[i]->model->Draw(RenderableObject::renderableObjects[i]->shader);
	}
}