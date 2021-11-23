#include "Camera.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 startingPosition, float fov) {
    this->cameraPosition = startingPosition;
    this->fov = fov;
}

void Camera::processKeyboardInput(GLFWwindow* window, float deltaTime) {
    float cameraSpeed = 10.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) cameraSpeed *= 2;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) cameraSpeed /= 2;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->cameraPosition += cameraSpeed * this->cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->cameraPosition -= cameraSpeed * this->cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->cameraPosition -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->cameraPosition += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * cameraSpeed;
}

void Camera::processMouseInput(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        this->lastX = (float) xpos;
        this->lastY = (float) ypos;
        this->firstMouse = false;
    }

    float xoffset = (float) xpos - lastX;
    float yoffset = lastY - (float) ypos;
    this->lastX = (float) xpos;
    this->lastY = (float) ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    if (this->pitch > 89.0f)  this->pitch = 89.0f;
    if (this->pitch < -89.0f) this->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    direction.y = sin(glm::radians(this->pitch));
    direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->cameraFront = glm::normalize(direction);
}

float Camera::getFOV() { return this->fov; }

glm::vec3& Camera::getCameraPosition() {
	return this->cameraPosition;
}
glm::vec3& Camera::getCameraFront() {
	return this->cameraFront;
}
glm::vec3& Camera::getCameraUp() {
	return this->cameraUp;
}

glm::mat4& Camera::getView() {
	this->view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	return this->view;
}