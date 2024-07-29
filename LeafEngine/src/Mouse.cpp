#include "Mouse.h"
#include "Window.h"
#include "Camera.h"

namespace lf {
	//there can be more mouse styles so that you can make more mouse classes
	//right now we don't have alternative mouse icons
	//but we have mouses that enable look around and those that do not
	//also mouses that enable scroll and those that do not
	//also, the mouse can be hidden
	static Mouse* currentMouse = nullptr;

	void Mouse::setCurrent()
	{
		currentMouse = this;
	}

	MousePosition Mouse::position;

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Camera::GetActiveCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	Mouse::Mouse(bool enableLookAround, bool enableScroll, bool hideMouse)
	{
		setCurrent();
		if (enableLookAround) glfwSetCursorPosCallback(Window::GetActiveWindow()->GetGLFWwindow(), MouseCallback);
		if (enableScroll) glfwSetScrollCallback(Window::GetActiveWindow()->GetGLFWwindow(), ScrollCallback);
		if (hideMouse) glfwSetInputMode(Window::GetActiveWindow()->GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Mouse::MouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
		static bool firstCall = false;
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		if (firstCall) {
			Mouse::position.x = xpos;
			Mouse::position.y = ypos;
			firstCall = false;
		}
		Camera::GetActiveCamera()->ProcessMouseMovement(xpos - Mouse::position.x, Mouse::position.y - ypos);
		Mouse::position.x = xpos;
		Mouse::position.y = ypos;
	}
}
