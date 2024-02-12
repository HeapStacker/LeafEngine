#ifndef CONTENT_INIT
#define CONTENT_INIT

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "camera.h"
#include "RenderableObject.h"

class ContentInitializer
{
    void initGLFW();
    GLFWwindow* createWindow(const char* windowName, int windowWidth, int windowHeight);
    void setCoreSettings(bool& enableLookAround, bool& enableScroll);
    void loadGlad();
    void setLastXY(unsigned int windowWidth, unsigned int windowHeight);
    ContentInitializer() {}
public:
    Camera* camera = nullptr; //refactor
    bool firstMouse = true;
    float lastX = 0.f;
    float lastY = 0.f;
	GLFWwindow* window = nullptr;
    ContentInitializer(ContentInitializer& other) = delete;
    void operator=(const ContentInitializer&) = delete;
    static ContentInitializer& GetInstance();
    void setUp(const char* contentName = "OpenGl Context", unsigned int windowWidth = 800, unsigned int windowHeight = 600, bool enableLookAround = false, bool enableScroll = false, glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f), float cameraFov = 45.f, float cameraSensitivity = 0.1f);
    ~ContentInitializer();
    void changeCamera(Camera* camera);
};


#endif

