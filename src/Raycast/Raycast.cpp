#include "Raycast.h"
#include "../Camera/Camera.h"

Raycast::Raycast(Camera* camera) :  camera(camera) { }

glm::vec3 Raycast::getEndingPosition(float distance) {
	glm::vec3 endingPosition = camera->getCameraPosition() + (distance * camera->getCameraFront());
	return endingPosition;
}