#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera(glm::vec3 startingPosition, float fov);

	void processKeyboardInput(GLFWwindow* window, float deltaTime);
	void processMouseInput(GLFWwindow* window, double xpos, double ypos);

	glm::vec3& getCameraPosition();
	glm::vec3& getCameraFront();
	glm::vec3& getCameraUp();
	glm::mat4& getView();

	float getFOV();
private:
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	float lastX = 400, lastY = 300;
	float yaw = -90.0f;
	float pitch = 0.0f;

	bool firstMouse = true;

	float fov = 45.0f;
};
