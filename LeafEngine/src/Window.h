#pragma once
#include "DllExporter.h"
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "FrameCapper.h"

namespace lf {

	class LF_LIB Window
	{
		GLFWimage images[1];
		GLFWwindow* window = nullptr;
		int width = 0, height = 0;
		bool fullScreen = false;
		bool shouldClose_ = false;
	public:
		Window(const char* name, unsigned int width, unsigned int height, const char* iconPath = "");
		void setPosition(unsigned int positionX, unsigned int positionY);
		static void getDesktopResolution(int& horizontal, int& vertical);
		void ceneterWindow();
		void togglefullScreen();
		bool isFullScreen() { return this->fullScreen; }
		void resizeOrChangeResolution(unsigned int width, unsigned int height);
		bool shouldClose();
		void activate();
		static Window* GetActiveWindow();
		GLFWwindow* GetGLFWwindow();
		int getWidth() { return this->width; }
		int getHeight() { return this->height; }
	};

}



