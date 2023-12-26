#include "leafEngine.h"

static bool down1 = false;
static bool up1 = false;
static bool down2 = false;
static bool up2 = false;
static bool mvRigh = true;
static bool mvUp = false;
static unsigned int firstColisionId;
static unsigned int secondColisionId;
static bool restart = true;
static bool stopped = true;
static float offset = 0.1f;

void processInput(Camera* camera, GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		down1 = true;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		up1 = true;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		down2 = true;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		up2 = true;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		restart = true;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		stopped = !stopped;
}

static void moveBall(RenderableObject& ball) {
	if (mvRigh) ball.setPosition(ball.getPosition() + glm::vec3(offset, 0.f, 0.f));
	else ball.setPosition(ball.getPosition() + glm::vec3(-offset, 0.f, 0.f));
	if (mvUp) ball.setPosition(ball.getPosition() + glm::vec3(0.f, offset, 0.f));
	else ball.setPosition(ball.getPosition() + glm::vec3(0.f, -offset, 0.f));
}

int main()
{
	GLFWwindow* window = nullptr;
	Camera* camera = nullptr;
	ContentInitializer& initializer = ContentInitializer::GetInstance();
	initializer.setUp("First game", 800, 300);
	setLighting({1, -1, -1}, DESERT);
	window = initializer.window;
	camera = initializer.camera;
	FrameCapper::SetFrameCap(120);
	Colider::setVisibility(false);


	RenderableObject paddle1(vertices, sizeof(vertices), { 1, 0.7, 0.2 });
	paddle1.setBoxColider();
	paddle1.setPosition({ -6, 0, -4 });
	paddle1.scale({ 0.5, 2, 0.3 });

	RenderableObject paddle2(vertices, sizeof(vertices), { 1, 0.7, 0.2 });
	paddle2.setBoxColider();
	paddle2.setPosition({ 6, 0, -4 });
	paddle2.scale({ 0.5, 2, 0.3 });

	RenderableObject ball(vertices, sizeof(vertices), "textures/container2.png", "textures/container2_specular.png");
	ball.setBoxColider();
	ball.setPosition({ 0, 0, -4 });
	ball.scale(0.4);

	RenderableObject wall1(vertices, sizeof(vertices), { 0.1, 0.7, 0.8 });
	wall1.setBoxColider();
	wall1.setPosition({ 0, 2.5, -4 });
	wall1.scale({ 12, 0.2, 1 });

	RenderableObject wall2(vertices, sizeof(vertices), { 0.1, 0.7, 0.8 });
	wall2.setBoxColider();
	wall2.setPosition({ 0, -2.5, -4 });
	wall2.scale({ 12, 0.2, 1 });

	RenderableObject flashLight(vertices, sizeof(vertices), { 0, 0, 0 });
	flashLight.setPosition(camera->Position + glm::vec3(0, -0.2, 0));
	flashLight.scale(0.2f);
	

	while (!glfwWindowShouldClose(window))
	{
		FrameCapper capper = FrameCapper();
		if (paddle1.getPosition().y > -2.5 && down1) {
			down1 = false;
			paddle1.translate({ 0.f, -offset, 0.f });
		}
		if (paddle1.getPosition().y < 2.5 && up1) {
			up1 = false;
			paddle1.translate({ 0.f, offset, 0.f });
		}
		if (paddle2.getPosition().y > -2.5 && down2) {
			down2 = false;
			paddle2.translate({ 0.f, -offset, 0.f });
		}
		if (paddle2.getPosition().y < 2.5 && up2) {
			up2 = false;
			paddle2.translate({ 0.f, offset, 0.f });
		}
		if (restart) { restart = false; ball.setPosition({ 0, 0, -4 }); }
		if (stopped) {
			static ColisionPair pair;
			static unsigned int firstColisionId, secondColisionId;
			pair = Colider::getLatestColision();
			firstColisionId = pair.firstColider;
			secondColisionId = pair.secondColider;
			if (firstColisionId != 0 && secondColisionId != 0) {
				if ((firstColisionId == paddle1.getID() || secondColisionId == paddle1.getID()) && (firstColisionId == ball.getID() || secondColisionId == ball.getID())) {
					mvRigh = !mvRigh;
				}
				if ((firstColisionId == paddle2.getID() || secondColisionId == paddle2.getID()) && (firstColisionId == ball.getID() || secondColisionId == ball.getID())) {
					mvRigh = !mvRigh;
				}
				if ((firstColisionId == wall1.getID() || secondColisionId == wall1.getID()) && (firstColisionId == ball.getID() || secondColisionId == ball.getID())) {
					if (mvUp) ball.translate({ 0.f, -offset, 0.f });
					else ball.translate({ 0.f, offset, 0.f });
					mvUp = !mvUp;
				}
				if ((firstColisionId == wall2.getID() || secondColisionId == wall2.getID()) && (firstColisionId == ball.getID() || secondColisionId == ball.getID())) {
					if (mvUp) ball.translate({ 0.f, -offset, 0.f });
					else ball.translate({ 0.f, offset, 0.f });
					mvUp = !mvUp;
				}
			}
			while (true) {
				static ColisionPair colPair;
				colPair = Colider::getLatestColision();
				if (colPair.firstColider == 0 && colPair.secondColider == 0) break;
			}
			moveBall(ball);
		}
		processInput(camera, window);
		RenderableObject::RenderObjects(window, camera);
	}
	glfwTerminate();
	return 0;
}