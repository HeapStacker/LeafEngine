#include "leafEngine.h"
#include <algorithm>
#include <sstream>
#include <random>

static bool jump = false, canJumpAgain = true;
static unsigned int windowWidth = 1200;
static unsigned int windowHeight = 600;
static float startPos = 6.4f, endPos = -6.4f;
static float pipeOffset = 3.5f;
static unsigned int sessionHighestPoints = 0;
static std::string sessionHighestPointsTxt = "Highest score : 0";
static unsigned int points = 0;
static std::string scoreTxt = "0";
static float pipeGap = 7.f;
static bool runGame = false;
static float fallSpeedAtOneSecond = 0.7f, fallDownSpeed = 0.f, terminalSpeed = 1.f;

void processInput(Camera* camera, GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { if (canJumpAgain) jump = true; canJumpAgain = false; runGame = true; }
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) canJumpAgain = true;
}

std::vector<int> pipeHeights = { -2, -3, -4, -5 };

void setPipes(std::vector<std::pair<RenderableObject*, RenderableObject*>>& pipes) {
	static bool firstInit = true;
	if (firstInit) {
		firstInit = false;
		for (int i = 0; i < pipes.size(); i++) {
			pipes[i].first->setBoxColider();
			pipes[i].first->scale({ 1, 5, 1 });
			pipes[i].second->setBoxColider();
			pipes[i].second->scale({ 1, 5, 1 });
		}
	}
	for (int i = 0; i < pipes.size(); i++) {
		int generatedHight = pipeHeights[rand() % pipeHeights.size()];
		pipes[i].first->setPosition({ startPos + pipeOffset * i, generatedHight, -4 });
		pipes[i].second->setPosition({ startPos + pipeOffset * i, generatedHight + pipeGap, -4 });
	}
}

void movePipes(std::vector<std::pair<RenderableObject*, RenderableObject*>>& pipes, std::pair<RenderableObject*, RenderableObject*>& lastPipes) {
	for (std::pair<RenderableObject*, RenderableObject*>& obj : pipes) {
		obj.first->translate({ -0.05f, 0, 0 });
		obj.second->translate({ -0.05f, 0, 0 });
		if (obj.first->getPosition().x <= endPos && lastPipes.first->getPosition().x + pipeOffset <= startPos) {
			lastPipes.first = obj.first;
			lastPipes.second = obj.second;
			int generatedHight = pipeHeights[rand() % pipeHeights.size()];
			lastPipes.first->setPosition({ startPos, generatedHight, -4 });
			lastPipes.second->setPosition({ startPos, generatedHight + pipeGap, -4 });
		}
		if (obj.first->getPosition().x > -0.02 && obj.first->getPosition().x < 0.02f) {
			points++;
			scoreTxt = std::to_string(points);
		}
	}
}


std::vector<std::pair<RenderableObject*, RenderableObject*>> pipes;
RenderableObject* birdObj = nullptr;

void restartGame() {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	if (points > sessionHighestPoints) sessionHighestPoints = points;
	points = 0;
	scoreTxt = std::to_string(points);
	std::stringstream ss;
	ss << "Highest score : " << sessionHighestPoints;
	sessionHighestPointsTxt = ss.str();
	runGame = false;
	setPipes(pipes);
	fallDownSpeed = 0.f;
	birdObj->setPosition({ 0, 0, -4 });
}

void Colision::DealWithColision(ColisionPair& pair) {
	if (pair.targetedColider == 1 || pair.transformedColider == 1) {
		restartGame();
	}
}

static glm::vec3 pipeCol = {0, 1, 0};
static glm::vec3 oldPipeCol = pipeCol;

void changePipeColor(std::vector<std::pair<RenderableObject*, RenderableObject*>>* pipes) {
	for (std::pair<RenderableObject*, RenderableObject*>& pipePair : *pipes) {
		pipePair.first->changeColor(pipeCol);
		pipePair.second->changeColor(pipeCol);
	}
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	srand((unsigned int)time(nullptr));
	GLFWwindow* window = nullptr;
	Camera* camera = nullptr;
	ContentInitializer& initializer = ContentInitializer::GetInstance();
	initializer.setUp("Flapy bird 3D", windowWidth, windowHeight, true);
	setLighting({ 1, 1, -1 }, BIOCHEMICAL_LAB);
	window = initializer.window;
	camera = initializer.camera;
	unsigned int fps = 60;
	FrameCapper::SetFrameCap(fps);


	GuiSelection scoreSelection;
	scoreSelection.setActivity(false);
	TransparentContainer scoreSelectionCont("score", windowWidth / 2 - 100, 10, 200);
	ChangeableText score(&scoreTxt, true);
	score.setFont("fonts/ChakraPetch-Regular.ttf", 60.f);
	SelectionEnd scoreSelectionEnd;

	GuiSelection sessionHighestScore;
	sessionHighestScore.setActivity(true);
	TransparentContainer sessionHighestScoreCont("sessionHighestScore", windowWidth - 450, 10, 500);
	ChangeableText sessionHighestScoreText(&sessionHighestPointsTxt, false, {0.14, 0.47, 0.17});
	sessionHighestScoreText.setFont("fonts/ChakraPetch-Regular.ttf", 60.f);
	SelectionEnd sessionHighestScoreEnd;

	GuiSelection pressSpaceSelection;
	pressSpaceSelection.setActivity(true);
	TransparentContainer pressSpaceSelectionCont("pressSpace", windowWidth / 2 - 200, windowHeight / 2, 400);
	Text pressSpaceSelectionContTxt("Press space to fly", true);
	pressSpaceSelectionContTxt.setFont("fonts/ChakraPetch-Regular.ttf", 40.f);
	SelectionEnd pressSpaceSelectionEnd;


	RenderableObject bird("models/bird/bird.obj");
	bird.setBoxColider();
	bird.setPosition({ 0, 0, -4 });
	bird.scale(0.03f);
	bird.rotateAround({ 1, 0, 0 }, 270.f);
	bird.rotateAround({ 0, 0, 1 }, 180.f);
	bird.getColider()->scale(10.f);
	bird.getColider()->translate({ 0, 0.1f, 0 });
	birdObj = &bird;
	RenderableObject downPipe1(vertices, sizeof(vertices), pipeCol);
	RenderableObject downPipe2(vertices, sizeof(vertices), pipeCol);
	RenderableObject downPipe3(vertices, sizeof(vertices), pipeCol);
	RenderableObject downPipe4(vertices, sizeof(vertices), pipeCol);
	RenderableObject upPipe1(vertices, sizeof(vertices), pipeCol);
	RenderableObject upPipe2(vertices, sizeof(vertices), pipeCol);
	RenderableObject upPipe3(vertices, sizeof(vertices), pipeCol);
	RenderableObject upPipe4(vertices, sizeof(vertices), pipeCol);
	RenderableObject backgroundImage(vertices, sizeof(vertices), "textures/background.png", "textures/background.png");
	backgroundImage.setPosition({ 0, 1.3f, -4.5f });

	float bkgMult = 2.f;
	backgroundImage.scale({ bkgMult * 6.08f, bkgMult * 4.57f, 1.f });
	pipes = { {&downPipe1, &upPipe1}, {&downPipe2, &upPipe2}, {&downPipe3, &upPipe3}, {&downPipe4, &upPipe4} };
	std::pair<RenderableObject*, RenderableObject*> lastPipes = pipes.back();
	setPipes(pipes);


	while (!glfwWindowShouldClose(window))
	{
		FrameCapper capper = FrameCapper();
		processInput(camera, window);

		if (pipeCol != oldPipeCol) {
			changePipeColor(&pipes);
		}
		if (runGame) {
			scoreSelection.setActivity(true);
			pressSpaceSelection.setActivity(false);
			if (jump && bird.getPosition().y < 2.f) { bird.translate({ 0, 0.7f, 0 }); jump = false; fallDownSpeed = 0.f; }
			else bird.translate({ 0, -fallDownSpeed, 0 });
			if (bird.getPosition().y <= -2.8f) restartGame();
			if (fallDownSpeed < terminalSpeed) fallDownSpeed += fallSpeedAtOneSecond / fps;
			movePipes(pipes, lastPipes);
		}
		else {
			scoreSelection.setActivity(false);
			pressSpaceSelection.setActivity(true);
		}

		RenderableObject::RenderObjects(window, camera);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}