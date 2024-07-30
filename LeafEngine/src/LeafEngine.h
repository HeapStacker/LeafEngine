//DLL USE xcopy / Y "$(OutDir)LeafEngine.dll" "$(SolutionDir)build\$(Platform)-$(Configuration)\playground\"

#pragma once
#include "DirectionalLight.h"
#include "GuiSelection.h"
#include "TexturedBox.h"
#include "FrameCapper.h"
#include "DllExporter.h"
#include "PointLight.h"
#include "ColoredBox.h"
#include "SpotLight.h"
#include "KeyInput.h"
#include "Collider.h"
#include "Camera.h"
#include "Window.h"
#include "Mouse.h"
#include "Model.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace lf {

#define APPLICATION_THREAD_START while (!Window::GetActiveWindow()->shouldClose()) {\
FrameCapper capper = FrameCapper();\
glClearColor(Shader::BackgroundColor.x, Shader::BackgroundColor.y, Shader::BackgroundColor.z, 1.0f);\
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);\
glm::mat4 projection = glm::perspective(glm::radians(Camera::GetActiveCamera()->Zoom), (float)Window::GetActiveWindow()->getWidth() / (float)Window::GetActiveWindow()->getHeight(), Shader::NearPlane, Shader::FarPlane);\
glm::mat4 view = Camera::GetActiveCamera()->GetViewMatrix();\
ImGui_ImplOpenGL3_NewFrame();\
ImGui_ImplGlfw_NewFrame();\
ImGui::NewFrame();

#define APPLICATION_THREAD_END ColoredBox::RenderColoredBoxes(view, projection);\
TexturedBox::RenderTexturedBoxes(view, projection);\
Model::RenderModels(view, projection);\
Collider::RenderColliders(view, projection);\
GuiContainer::RenderGui();\
Collider::CheckForCollisions();\
Collider::ResolveCollisions();\
glfwSwapBuffers(Window::GetActiveWindow()->GetGLFWwindow());\
glfwPollEvents();\
}

}


