#pragma once

class Camera;
#include <glm/glm.hpp>

class Raycast {
public:
	Raycast(Camera* camera);
	glm::vec3 getEndingPosition(float distance);

private:
	Camera* camera = nullptr;
};