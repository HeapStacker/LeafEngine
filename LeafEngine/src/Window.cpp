#include "Window.h"
#include "Shader.h"
#include "stb_image.h"
#include "GuiSelection.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


namespace lf {
	Window* activeWindow = nullptr;

	Window::Window(const char* name, unsigned int width, unsigned int height, const char* iconPath) {
		static bool firstCall = true;
		if (firstCall) {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		}
		this->width = width;
		this->height = height;
		this->window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (!this->window) {
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(1);
		}
		activate();
		if (iconPath) {
			GLFWimage images[1];
			images[0].pixels = stbi_load(iconPath, &images[0].width, &images[0].height, 0, 4); //rgba channels 
			glfwSetWindowIcon(this->window, 1, images);
			stbi_image_free(images[0].pixels);
		}
		glfwGetWindowSize(this->window, &this->width, &this->height);
		ceneterWindow();
		if (firstCall) {
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initialize GLAD" << std::endl;
				exit(2);
			}
			stbi_set_flip_vertically_on_load(true);
			glEnable(GL_DEPTH_TEST);

			Shader::initializeShaders();

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			(void)io;
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(Window::GetActiveWindow()->GetGLFWwindow(), true);
			ImGui_ImplOpenGL3_Init("#version 330");

			//novo...
			io.WantCaptureMouse = true;
		}
		firstCall = false;
	}

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Window::activate()
	{
		activeWindow = this;
		glfwMakeContextCurrent(this->window);
		glfwSetFramebufferSizeCallback(this->window, FramebufferSizeCallback);
	}

	Window* Window::GetActiveWindow()
	{
		return activeWindow;
	}

	GLFWwindow* Window::GetGLFWwindow()
	{
		if (activeWindow->window) {
			return activeWindow->window;
		}
		else {
			std::cerr << "There are no active windows selected!\n" << std::endl;
			glfwTerminate();
			exit(4);
		}
	}

	void Window::setPosition(unsigned int positionX, unsigned int positionY)
	{
		glfwSetWindowPos(this->window, positionX - this->width / 2, positionY - this->height / 2);
	}

	void Window::ceneterWindow()
	{
		int horizontal = 0, vertical = 0;
		getDesktopResolution(horizontal, vertical);
		setPosition(horizontal / 2, vertical / 2);
	}

	void Window::resizeOrChangeResolution(unsigned int width, unsigned int height)
	{
		glfwSetWindowSize(this->window, width, height);
	}

	void Window::togglefullScreen()
	{
		if (!this->fullScreen) {
			int horizontal = 0, vertical = 0;
			getDesktopResolution(horizontal, vertical);
			glfwSetWindowMonitor(this->window, glfwGetPrimaryMonitor(), 0, 0, horizontal, vertical, FrameCapper::GetFrameCap());
		}
		else { glfwSetWindowMonitor(this->window, NULL, 0, 0, this->width, this->height, 0); }
		this->fullScreen = !this->fullScreen;
	}

	bool Window::shouldClose() {
		return glfwWindowShouldClose(this->window);
	}

#if LF_WINDOWS
#include <Windows.h>
	void Window::getDesktopResolution(int& horizontal, int& vertical)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		horizontal = desktop.right;
		vertical = desktop.bottom;
	}
#else
	//does nothing yet (platform not defined)
	static void getDesktopResolution(int& horizontal, int& vertical) {}
	void centerWindow() {}
#endif

}