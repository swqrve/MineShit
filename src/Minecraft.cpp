#include "Minecraft.h"

#include "./Game/Game.h"

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

Game* minecraft;

double previousTime = glfwGetTime();
double previousFrame = glfwGetTime();

int frameCount = 0;
float deltaTime = 0;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minecraft", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		std::cin.get();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, windowChangeSizeEvent);

	glfwSetScrollCallback(window, scrollEvent);
	glfwSetCursorPosCallback(window, mouseMoveEvent);
	glfwSetMouseButtonCallback(window, mouseClickEvent);

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glfwSwapInterval(0); // Disable vSync, pretty much unlimit fps (Change to 1 to limit fps to 60, Change to 0 to unlimit FPS)

	minecraft = new Game(1280, 720);

	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		deltaTime = (float) (currentTime - previousFrame);

		previousFrame = currentTime;
		frameCount++;

		if (currentTime - previousTime >= 1.0) {
			glfwSetWindowTitle(window, std::to_string(frameCount).c_str());

			frameCount = 0;
			previousTime = currentTime;
		}

		minecraft->processKeyboardInput(window, deltaTime);

		glClearColor(0.529f, 0.80f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		minecraft->update(deltaTime);
		minecraft->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void windowChangeSizeEvent(GLFWwindow* window, int width, int height) { glfwSetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT); }

void scrollEvent(GLFWwindow* window, double xoffset, double yoffset) { minecraft->processScrollInput(window, xoffset, yoffset, deltaTime); }

void mouseMoveEvent(GLFWwindow* window, double xpos, double ypos) { minecraft->processMouseMovement(window, xpos, ypos, deltaTime); }

void mouseClickEvent(GLFWwindow* window, int button, int action, int mods) { minecraft->mouseClickInput(window, button, action, mods, deltaTime); };