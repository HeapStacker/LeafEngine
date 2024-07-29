#pragma once
#include "DllExporter.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace lf {
	struct MousePosition {
		float x = 0;
		float y = 0;
	};

	class LF_LIB Mouse
	{
		static MousePosition position;
	public:

		//create a mouse after the window has been properly created!
		//it's a good practice to also first create a camera, then a mouse!
		Mouse(bool enableLookAround, bool enableScroll, bool hideMouse);

		void setCurrent();

		static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	};
}

