#include "KeyInput.h"
#include "Window.h"

namespace lf {
	bool keyPressed(int keyCode)
	{
		return glfwGetKey(Window::GetActiveWindow()->GetGLFWwindow(), keyCode) == GLFW_PRESS;
	}

	bool keyReleased(int keyCode)
	{
		return glfwGetKey(Window::GetActiveWindow()->GetGLFWwindow(), keyCode) == GLFW_RELEASE;
	}
}