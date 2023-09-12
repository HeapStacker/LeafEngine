#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using uint = unsigned int;
class ContentInitializer
{
	GLFWwindow* window = nullptr;
public:
	ContentInitializer(const char* processName, bool openInitializer = true, uint scrWidth = 800, uint scrHeight = 600, uint fpsCap = 60, float fov = 45.f, float sensitivity = 0.1f) {
	}
	~ContentInitializer() {

	}
};