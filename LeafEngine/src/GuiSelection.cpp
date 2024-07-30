#include "Window.h"
#include <iostream>
#include "ModelLoader.h"
#include "GuiSelection.h"
#include "imgui_impl_opengl3.h"

namespace lf {

	std::vector<GuiContainer*> GuiContainer::containers;

	void GuiContainer::changeProperties(bool hasBackground, bool canMove, bool canResize) {
		flags = 0;
		this->flags |= ImGuiWindowFlags_AlwaysAutoResize;
		this->flags |= ImGuiWindowFlags_NoTitleBar;
		this->flags |= ImGuiWindowFlags_NoCollapse;
		if (!hasBackground) this->flags |= ImGuiWindowFlags_NoBackground;
		if (!canMove) this->flags |= ImGuiWindowFlags_NoMove;
		if (!canResize) this->flags |= ImGuiWindowFlags_NoResize;
	}
	void GuiContainer::center() {
		if (width && height) {
			posX = (Window::GetActiveWindow()->getWidth() - width) / 2.f;
			posY = (Window::GetActiveWindow()->getHeight() - height) / 2.f;
			canDisplace = true;
		}
		else {
			std::cerr << "Container width & height aren't specified.\n";
		}
	}
	void GuiContainer::changeSize(float width, float height) {
		this->width = width;
		this->height = height;
		canResize = true;
	}
	void GuiContainer::changePosition(float posX, float posY) {
		this->posX = posX;
		this->posY = posY;
		canDisplace;
	}
	GuiContainer::GuiContainer(const char* identificator, float posX, float posY, float width, float height) {
		this->identificator = identificator;
		this->posX = posX;
		this->posY = posY;
		this->width = width;
		this->height = height;
		this->flags |= ImGuiWindowFlags_AlwaysAutoResize;
		this->flags |= ImGuiWindowFlags_NoTitleBar;
		this->flags |= ImGuiWindowFlags_NoCollapse;
		this->flags |= ImGuiWindowFlags_NoResize;
		this->flags |= ImGuiWindowFlags_NoMove;
		containers.push_back(this);
	}
	void GuiContainer::changeBackgroundColor(const glm::vec4& color) { this->color = color; }
	void GuiContainer::RenderGui() {
		for (GuiContainer* cnt : containers) {
			if (cnt->visiblity) cnt->render();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void GuiContainer::setVisibility(const bool& visiblity) {
		this->visiblity = visiblity;
	}
	void GuiContainer::addElement(GuiElement* element) {
		subElements.push_back(element);
	}
	void GuiContainer::render() {
		if (canDisplace) {
			ImGui::SetNextWindowPos({ posX, posY });
			canDisplace = false;
		}
		if (canResize) {
			ImGui::SetNextWindowSize({ width, height });
			canResize = false;
		}
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(color[0], color[1], color[2], color[3]));
		ImGui::Begin(identificator, nullptr, flags);
		ImGui::PopStyleColor();
		for (GuiElement* el : subElements) el->manifest();
		ImGui::End();
	}

	ImFont* FontChanger::currentFont = nullptr;
	void FontChanger::ChangeFont(const char* path, float pxSize) {
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontDefault();
		currentFont = io.Fonts->AddFontFromFileTTF(path, pxSize);
		if (!currentFont) {
			std::cerr << "Font not found!\n";
		}
	}

	Text::Text(std::string text, bool centered, glm::vec3 color) {
		if (text.size()) this->text = text;
		else std::cerr << "String isn't properly assigned!\n";
		this->centered = centered;
		this->color = color;
		GuiContainer::containers.back()->addElement(this);
	}
	void Text::changeFontColor(const glm::vec3& color) { this->color = color; }
	void Text::manifest() {
		if (FontChanger::currentFont) { ImGui::PushFont(FontChanger::currentFont); }
		if (centered) { ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text.c_str()).x) * 0.5f); }
		ImGui::TextColored(ImVec4(color[0], color[1], color[2], 1.f), text.c_str());
		if (FontChanger::currentFont) { ImGui::PopFont(); }
	}

	ChangeableText::ChangeableText(std::string* text, bool centered, glm::vec3 color) {
		if (text && text->size()) this->text = text;
		else std::cerr << "String isn't properly assigned!\n";
		this->centered = centered;
		this->color = color;
		GuiContainer::containers.back()->addElement(this);
	}
	void ChangeableText::changeFontColor(const glm::vec3& color) { this->color = color; }
	void ChangeableText::manifest() {
		if (FontChanger::currentFont) { ImGui::PushFont(FontChanger::currentFont); }
		if (centered) { ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text->c_str()).x) * 0.5f); }
		ImGui::TextColored(ImVec4(color[0], color[1], color[2], 1.f), text->c_str());
		if (FontChanger::currentFont) { ImGui::PopFont(); }
	}

	CheckBox::CheckBox(std::string text, bool state) {
		this->text = text;
		this->state = state;
		GuiContainer::containers.back()->addElement(this);
	}
	void CheckBox::manifest() {
		ImGui::Checkbox(text.c_str(), &state);
	}

	ColorEditor::ColorEditor(std::string text, glm::vec3* color) {
		this->text = text;
		this->color = color;
		GuiContainer::containers.back()->addElement(this);
	}
	void ColorEditor::manifest() {
		ImGui::ColorEdit4(text.c_str(), (float*)color);
	}

	Slider::Slider(const char* descritpion, float* trackerValue, float min, float max) {
		this->descritpion = descritpion;
		this->trackerValue = trackerValue;
		this->min = min;
		this->max = max;
		GuiContainer::containers.back()->addElement(this);
	}
	void Slider::manifest() {
		ImGui::SliderFloat(descritpion, trackerValue, min, max);
	}


	Button::Button(const char* innerText, float width, float height, float rounding) {
		this->innerText = innerText;
		this->width = width;
		this->height = height;
		this->rounding = rounding;
		GuiContainer::containers.back()->addElement(this);
	}
	void Button::setCallback(ButtonCallback callback) {
		this->callback = callback;
	}
	void Button::changeColors(const ImVec4& baseColor, const ImVec4& hoveredColor, const ImVec4& clickedColor) {
		this->baseColor = baseColor;
		this->hoveredColor = hoveredColor;
		this->clickedColor = clickedColor;
	}
	void Button::manifest() {
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::PushStyleColor(ImGuiCol_Button, baseColor);       
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, clickedColor); 

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); // Postavljanje paddinga na nulu
		if (ImGui::Button(innerText, ImVec2(width, height))) {
			if (this->callback) {
				this->callback();
			}
		}
		ImGui::PopStyleVar();

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
	}

	Image::Image(const char* path, float width, float height) {
		this->texture = internal::TextureFromFile(path, ".", false, true);
		this->width = width;
		this->height = height;
		GuiContainer::containers.back()->addElement(this);
	}
	void Image::changeTexture(const char* path) {
		this->texture = internal::TextureFromFile(path);
	}
	void Image::changeWidthHeight(float width, float height) {
		this->width = width;
		this->height = height;
	}
	void Image::manifest() {
		ImGui::Image((void*)(intptr_t)this->texture, ImVec2(width, height));
	}
}