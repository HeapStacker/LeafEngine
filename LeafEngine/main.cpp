#include <glad/glad.h>
#include <GLFW/glfw3.h>+
#include "camera.h"
#include "contentInitializer.h"
#include "RenderableObject.h"
#include "manualSettings.h"
#include "FrameCapper.h"+

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window);

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLFWwindow* window = nullptr;
Camera* camera = nullptr;

int main()
{
	float dz = 0.1f;
	ContentInitializer& initializer = ContentInitializer::GetInstance();
	initializer.setUp("OpenGL Tut", 800, 600);
	window = initializer.window;
	camera = initializer.camera;
	RenderableObject::SetLightProperties(dirLight, spotLight);


	
	RenderableObject backpack("models/backpack/backpack.obj");
	backpack.setPosition({ 10.f, 3.f, -2.f });
	backpack.rotateAround({ 1.f, 0.f, 1.f }, 37.f);
	backpack.setBoxColider({3, 3, 3});
	Colider::setVisibility(true);
	RenderableObject cubeBag("models/backpack/backpack.obj");
	cubeBag.setPosition({ 10.f, 3.f, -10.f });
	cubeBag.setSphereColider(2);
	cubeBag.scale(0.2f);
	RenderableObject smalObj(vertices, sizeof(vertices), { 1, 0, 1 });
	smalObj.setSphereColider(0.9f);
	smalObj.setPosition({ 0, 0, 0 });
	RenderableObject smallerObj(vertices, sizeof(vertices), { 1, 0, 1 });
	smallerObj.setBoxColider({ 1, 1, 1 });
	smallerObj.setPosition({ 1, 0, 0 });
	smallerObj.scale(0.5);
	smallerObj.getColider()->scale(3);

	deltaTime = 1.0 / FrameCapper::GetFrameCap(); //1.f (not 1000.f because delta time needs to be in respect with seconds)

	while (!glfwWindowShouldClose(window))
	{
		FrameCapper capper();
		if (cubeBag.getPosition().z <= -20 || cubeBag.getPosition().z >= 10) {
			dz = dz * -1.f;
		}
		cubeBag.setPosition(cubeBag.getPosition() + glm::vec3(0.f, 0.f, dz));
		processInput(window);
		RenderableObject::RenderObjects(window, camera);
	}
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS) {
		RenderableObject::Erase(2);
	}
}