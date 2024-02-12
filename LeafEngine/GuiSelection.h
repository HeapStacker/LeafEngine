#pragma once
#include <vector>
#include <string>
#include "imgui.h"
#include <glm/glm.hpp>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

#define MAX_UI_ELEMENT_SIZE ImVec2(1280, 720)

class GuiElement;
void addGuiElementToSelection(GuiElement* element);

class GuiElement {
	GuiElement* element = this;
public:
	//first create GuiSelection than start creating GuiElements to bind them to the right selection
	GuiElement() {
		addGuiElementToSelection(element);
	}
	virtual void manifest() = 0;
	GuiElement* getGuiElement() { return this; }
};

class TransparentContainer : public GuiElement {
	float posX = 0.f, posY = 0.f, width = 0.f;
	const char* identificator;
public:
	//dont change width if you dont want the window to be centered
	TransparentContainer(const char* identificator, float posX = 0.f, float posY = 0.f, float width = 0.f) {
		this->identificator = identificator;
		this->posX = posX;
		this->posY = posY;
		this->width = width;
	}
	float getWidth() {
		if (width) return width;
		else std::cerr << "Window width is not manualy defined!\n";
	}
	void manifest() override {
		ImGui::SetNextWindowPos(ImVec2(posX, posY));
		if (width) {
			ImGui::SetNextWindowSize(ImVec2(width, -1));
		}
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoMove;
		ImGui::Begin(identificator, nullptr, window_flags);
	}
};

class ColoredContainer : public GuiElement {
	float posX = 0.f, posY = 0.f, width = 0.f;
	glm::vec3 color = { 0.4f, 0.4f, 0.6f };
	const char* identificator;
public:

	//if you want to center the window and its text
	ColoredContainer(const char* identificator, float posX = 0.f, float posY = 0.f, glm::vec3 color = { 0.4f, 0.4f, 0.6f }, float width = 0.f) {
		this->identificator = identificator;
		this->posX = posX;
		this->posY = posY;
		this->color = color;
		this->width = width;
	}
	float getWidth() {
		if (width) return width;
		else std::cerr << "Window width is not manualy defined!\n";
	}
	void changeBackgroundColor(const glm::vec3& color) { this->color = color; }
	void manifest() override {
		ImGui::SetNextWindowPos(ImVec2(posX, posY));
		if (width) {
			ImGui::SetNextWindowSize(ImVec2(width, -1));
		}
		/*ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), MAX_UI_ELEMENT_SIZE);*/
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoMove;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(color[0], color[1], color[2], 1.f));
		ImGui::Begin(identificator, nullptr, window_flags);
		ImGui::PopStyleColor();
	}
};

class Text : public GuiElement {
	std::string text;
	bool centered = false;
	ImFont* currentFont = nullptr;
	glm::vec3 color = { 1.f, 1.f, 1.f };
public:
	Text(std::string text, bool centered = false, glm::vec3 color = { 1.f, 1.f, 1.f }) {
		if (text.size()) this->text = text;
		else std::cerr << "String isn't properly assigned!\n";
		this->centered = centered;
		this->color = color;
	}

	//for ttf fonts...
	void setFont(const char* path, float pxSize) {
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontDefault();
		currentFont = io.Fonts->AddFontFromFileTTF(path, pxSize);
		if (!currentFont) {
			std::cerr << "Font not found!\n";
		}
	}
	void changeFontColor(const glm::vec3& color) { this->color = color; }
	void manifest() override {
		if (currentFont) { ImGui::PushFont(currentFont); }
		if (centered) { ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) * 0.5f); }
		ImGui::TextColored(ImVec4(color[0], color[1], color[2], 1.f), text.c_str());
		if (currentFont) { ImGui::PopFont(); }
	}
};

class ChangeableText : public GuiElement {
	std::string* text = nullptr;
	bool centered = false;
	ImFont* currentFont = nullptr;
	glm::vec3 color = { 1.f, 1.f, 1.f };
public:
	ChangeableText(std::string* text, bool centered = false, glm::vec3 color = { 1.f, 1.f, 1.f }) {
		if (text && text->size()) this->text = text;
		else std::cerr << "String isn't properly assigned!\n";
		this->centered = centered;
		this->color = color;
	}

	//for ttf fonts...
	void setFont(const char* path, float pxSize) {
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontDefault();
		currentFont = io.Fonts->AddFontFromFileTTF(path, pxSize);
		if (!currentFont) {
			std::cerr << "Font not found!\n";
		}
	}
	void changeFontColor(const glm::vec3& color) { this->color = color; }
	void manifest() override {
		if (currentFont) { ImGui::PushFont(currentFont); }
		if (centered) { ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text->c_str()).x) * 0.5f); }
		ImGui::TextColored(ImVec4(color[0], color[1], color[2], 1.f), text->c_str());
		if (currentFont) { ImGui::PopFont(); }
	}
};

class CheckBox : public GuiElement {
	bool* state = nullptr;
	std::string text;
public:
	CheckBox(std::string text, bool* state) {
		this->text = text;
		this->state = state;
	}
	void manifest() override {
		ImGui::Checkbox(text.c_str(), state);
	}
};

class ColorEditor : public GuiElement {
	std::string text;
	glm::vec3* color;
public:
	ColorEditor(std::string text, glm::vec3* color) {
		this->text = text;
		this->color = color;
	}
	void manifest() override {
		ImGui::ColorEdit4(text.c_str(), (float*)color);
	}
};

class Slider : public GuiElement {
	const char* descritpion;
	float* trackerValue = nullptr;
	float min, max;
public:
	Slider(const char* descritpion, float* trackerValue, float min, float max) {
		this->descritpion = descritpion;
		this->trackerValue = trackerValue;
		this->min = min;
		this->max = max;
	}
	void manifest() override {
		ImGui::SliderFloat(descritpion, trackerValue, min, max);
	}
};

class SelectionEnd : public GuiElement {
public:
	SelectionEnd() {}
	void manifest() override {
		ImGui::End();
	}
};

void addSelectionToRender(unsigned int selectionId);

class GuiSelection
{
	unsigned int id;
	static std::vector<GuiSelection*> selections;
	std::vector<GuiElement*> elements;
	static unsigned int idTracker;
	bool active = false;
public:
	GuiSelection() {
		id = ++idTracker;
		selections.push_back(this);
		addSelectionToRender(id);
	}

	static GuiSelection* FindSelectionById(unsigned int id);

	void addGuiElement(GuiElement* element) {
		elements.push_back(element);
	}

	void renderElements() {
		if (active) {
			for (GuiElement* element : elements) {
				element->manifest();
			}
		}
	}

	void setActivity(bool activity) {
		active = activity;
	}

	unsigned int getId() { return id; }
	static unsigned int getLastId();
};