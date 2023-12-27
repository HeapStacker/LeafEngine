#include "leafEngine.h"

static bool down1 = false;
static bool up1 = false;
static bool down2 = false;
static bool up2 = false;
static bool mvRigh = true;
static bool mvUp = false;
static unsigned int firstColisionId;
static unsigned int secondColisionId;
static float offset = 0.1f;
static unsigned int w1 = 0;
static unsigned int w2 = 0;

void processInput(Camera* camera, GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		down2 = true;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		up2 = true;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		up1 = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		down1 = true;
}

static void moveBall(RenderableObject& ball) {
	if (mvRigh) ball.setPosition(ball.getPosition() + glm::vec3(offset, 0.f, 0.f));
	else ball.setPosition(ball.getPosition() + glm::vec3(-offset, 0.f, 0.f));
	if (mvUp) ball.setPosition(ball.getPosition() + glm::vec3(0.f, offset, 0.f));
	else ball.setPosition(ball.getPosition() + glm::vec3(0.f, -offset, 0.f));
	if (ball.getPosition().x <= -6.f) {
		w2++;
		std::cout << "Right player won!\nResult:" << w1 << "\t" << w2 << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
#ifdef __linux__ 
		system("clear");
#elif _WIN32
		system("cls");
#else
#endif
		ball.setPosition({ 0, 0, -4 });
	}
	if (ball.getPosition().x >= 6.f) {
		w1++;
		std::cout << "Left player won!\nResult:" << w1 << "\t" << w2 << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
#ifdef __linux__ 
		system("clear");
#elif _WIN32
		system("cls");
#else
#endif
		ball.setPosition({ 0, 0, -4 });
	}
}

//this screams to be refactored
void Colision::DealWithColision(ColisionPair& pair) {
	static RenderableObject* ball = RenderableObject::FindById(3); 
	if (pair.transformedColider == 3) {
		if (pair.targetedColider == 1 || pair.targetedColider == 2) {
			mvRigh = !mvRigh;
		}
		if (pair.targetedColider == 4 || pair.targetedColider == 5) {
			if (mvUp) ball->translate({ 0.f, -offset, 0.f });
			else ball->translate({ 0.f, offset, 0.f });
			mvUp = !mvUp;
		}
	}
}

int main()
{
	GLFWwindow* window = nullptr;
	Camera* camera = nullptr;
	ContentInitializer& initializer = ContentInitializer::GetInstance();
	initializer.setUp("First game", 800, 300);
	setLighting({1, -1, -1}, NORMAL);
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
		moveBall(ball);
		processInput(camera, window);
		RenderableObject::RenderObjects(window, camera);
	}
	glfwTerminate();
	return 0;
}