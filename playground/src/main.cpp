#include "LeafEngine.h"

using namespace lf;

static unsigned int windowWidth = 800;
static unsigned int windowHeight = 600;
unsigned int fps = 60;
bool moveF = false, moveL = false, moveR = false, moveB = false;
Window gameWindow("Playground", windowWidth, windowHeight, "images/ico.png");
Camera gameCamera({ 0, 0, 3 });
Mouse mouse(false, false, false);
ColoredBox box({ 1.f, 1.f, 1.f });
Model bird("models/bird/bird.obj");
TexturedBox box1 = TexturedBox("textures/background.png", "textures/background.png");
TexturedBox box2 = TexturedBox("textures/background.png", "textures/background.png");
TexturedBox box3 = TexturedBox("textures/background.png", "textures/background.png");

void processInput()
{
	if (keyPressed(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(Window::GetActiveWindow()->GetGLFWwindow(), true);
	if (keyPressed(GLFW_KEY_W)) box.translate({ 0, 0, -0.1f }); moveF = false;
	if (keyPressed(GLFW_KEY_A)) box.translate({ -0.1f, 0, 0 }); moveL = false;
	if (keyPressed(GLFW_KEY_D)) box.translate({ 0.1f, 0, 0 }); moveR = false;
	if (keyPressed(GLFW_KEY_S)) box.translate({ 0, 0, 0.1f }); moveB = false;
}

void colisionRulles(ColisionPair& pair) {
	std::cout << "COLISION BETWEEN:\t" << pair.id1 << "\t" << pair.id2 << std::endl;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	FrameCapper::SetFrameCap(fps);
	Collider::SetVisibility(true);
	Collider::AssignCollisionRules(colisionRulles);

	box.rotateAround({ 1, 1, 0 }, 45.f);
	box.setPosition({ -5, 0, 1 });

	bird.setPosition({ 3.f, 0.f, -10.f });
	bird.scale(0.3f);
	bird.rotateAround({ 1, 0, 0 }, 270);
	Collider col1 = Collider();
	Collider col2 = Collider();
	col1.setPosition(bird.getPosition());
	col2.setPosition(box.getPosition());
	col1.linkToPosition(&bird);
	col2.linkToPosition(&box);

	//DirectionalLight redSun = DirectionalLight({ 1, 1, 1 });
	PointLight lamp = PointLight(box.getPosition());
	lamp.linkToPosition(&box);
	box1.setPosition({ 2.f, 0, -15.f });
	box2.setPosition({ -1.f, 0, -15.f });
	box3.setPosition({ -2.f, 0, -15.f });
	box2.linkToRotation(&box1);
	box2.scale(2.f);
	box3.linkToRotation(&box1);

	GuiContainer cont("win3", 10, 10, 400, 200);
	cont.center();
	Text txt("My transparent container");
	CheckBox cb1("Free robux");
	glm::vec3 color = { 0, 0, 0 };
	ColorEditor edit("Pick a color", &color);
	CheckBox cb2("Free robux for friends");
	Button btn("dijsidjas", 40, 20);

	APPLICATION_THREAD_START
	processInput();
	box1.rotateAround({ 0, 0, 1 }, 1);
	APPLICATION_THREAD_END

	return 0;
}