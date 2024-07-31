#pragma once
#include "DllExporter.h"
#include <vector>
#include <string>
#include "imgui.h"
#include <glm/glm.hpp>

namespace lf {

	class LF_LIB GuiElement {
		GuiElement* element = this;
	public:
		GuiElement() {}
		virtual void manifest() = 0;
	};

	class LF_LIB GuiContainer {
		const char* identificator;
		ImGuiWindowFlags flags = 0;
		std::vector<GuiElement*> subElements;
		glm::vec4 color = { 0.f, 0.f, 0.1f, 0.2f };
		float posX = 0.f, posY = 0.f, width = 0.f, height = 0.f;
		bool visiblity = true, canDisplace = true, canResize = true;
	public:
		static void RenderGui();
		static std::vector<GuiContainer*> containers;
		void changeProperties(bool hasBackground, bool canMove, bool canResize);
		void center();
		void changeSize(float width = 0.f, float height = 0.f);
		void changePosition(float posX = 0.f, float posY = 0.f);
		GuiContainer(const char* identificator, float posX = 0.f, float posY = 0.f, float width = 0.f, float height = 0.f);
		void changeBackgroundColor(const glm::vec4& color);
		void setVisibility(const bool& visiblity);
		void addElement(GuiElement* element);
		void render();
	};

	class LF_LIB FontChanger {
	public:
		static ImFont* currentFont;
		static void ChangeFont(const char* path, float pxSize);
	};

	class LF_LIB Text : public GuiElement {
		std::string text;
		bool centered = false;
		glm::vec3 color = { 1.f, 1.f, 1.f };
	public:
		Text(std::string text, bool centered = false, glm::vec3 color = { 1.f, 1.f, 1.f });
		void changeFontColor(const glm::vec3& color);
		void manifest() override;
	};

	class LF_LIB ChangeableText : public GuiElement {
		std::string* text = nullptr;
		bool centered = false;
		glm::vec3 color = { 1.f, 1.f, 1.f };
	public:
		ChangeableText(std::string* text, bool centered = false, glm::vec3 color = { 1.f, 1.f, 1.f });
		void changeFontColor(const glm::vec3& color);
		void manifest() override;
	};

	class LF_LIB CheckBox : public GuiElement {
		std::string text;
	public:
		bool state = false;
		CheckBox(std::string text, bool state = false);
		void manifest() override;
	};

	class LF_LIB ColorEditor : public GuiElement {
		std::string text;
		glm::vec3* color;
	public:
		ColorEditor(std::string text, glm::vec3* color);
		void manifest() override;
	};

	class LF_LIB Slider : public GuiElement {
		const char* descritpion;
		float* trackerValue = nullptr;
		float min, max;
	public:
		Slider(const char* descritpion, float* trackerValue, float min, float max);
		void manifest() override;
	};

	using ButtonCallback = void(*)();

	class LF_LIB Button : public GuiElement {
		const char* innerText;
		ButtonCallback callback;
		float width, height, rounding;
		ImVec4 baseColor = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
		ImVec4 hoveredColor = ImVec4(0.2f, 0.7f, 1.0f, 1.0f);
		ImVec4 clickedColor = ImVec4(0.0f, 0.3f, 0.8f, 1.0f);
	public:
		Button(const char* innerText, float width = 90.f, float height = 30.f, float rounding = 5.f);
		void setCallback(ButtonCallback callback);
		void changeColors(const ImVec4& baseColor, const ImVec4& hoveredColor, const ImVec4& clickedColor);
		void manifest() override;
	};

	class LF_LIB Image : public GuiElement {
		unsigned int texture = 0;
		int width, height;
		float scale = 1.f;
	public:
		Image(const char* path, float scale = 1.f);
		void changeScale(float scale);
		void changeTexture(const char* path);
		void changeWidthHeight(float width, float height);
		void manifest() override;
	};
}