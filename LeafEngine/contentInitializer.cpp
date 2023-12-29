#include "contentInitializer.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


void ContentInitializer::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWwindow* ContentInitializer::createWindow(const char* windowName, int windowWidth, int windowHeight) {
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwSetWindowPos(window, 800, 400); //just for setting window position manualy (for now)
    return window;
}

void ContentInitializer::loadGlad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(2);
    }
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
}

void ContentInitializer::setLastXY(unsigned int windowWidth, unsigned int windowHeight) {
    lastX = (float)windowWidth;
    lastY = (float)windowHeight;
}

void ContentInitializer::setCoreSettings(bool& enableLookAround, bool& enableScroll) {
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (enableLookAround) glfwSetCursorPosCallback(window, mouse_callback);
    if (enableScroll) glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

//memory leak fixed
ContentInitializer& ContentInitializer::GetInstance()
{
    static ContentInitializer contentInitializer = ContentInitializer();
    return contentInitializer;
}

void ContentInitializer::setUp(const char* contentName, unsigned int windowWidth, unsigned int windowHeight, bool enableLookAround, bool enableScroll, glm::vec3 cameraPos, float cameraFov, float cameraSensitivity) {
    initGLFW();
    setLastXY(windowWidth, windowHeight); //fix maby
    RenderableObject::SetWindowWidthHeight(windowWidth, windowHeight);
    window = createWindow(contentName, windowWidth, windowHeight);
    setCoreSettings(enableLookAround, enableScroll);
    loadGlad();
    camera = new Camera(cameraPos, cameraFov, cameraSensitivity);
}

ContentInitializer::~ContentInitializer() {
    delete camera;
}

void ContentInitializer::changeCamera(Camera* camera) {
    delete(this->camera);
    this->camera = nullptr;
    this->camera = camera;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (ContentInitializer::GetInstance().firstMouse)
    {
        ContentInitializer::GetInstance().lastX = xpos;
        ContentInitializer::GetInstance().lastY = ypos;
        ContentInitializer::GetInstance().firstMouse = false;
    }
    float xoffset = xpos - ContentInitializer::GetInstance().lastX;
    float yoffset = ContentInitializer::GetInstance().lastY - ypos;
    ContentInitializer::GetInstance().lastX = xpos;
    ContentInitializer::GetInstance().lastY = ypos;
    ContentInitializer::GetInstance().camera->ProcessMouseMovement(xoffset, yoffset);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ContentInitializer::GetInstance().camera->ProcessMouseScroll(static_cast<float>(yoffset));
}