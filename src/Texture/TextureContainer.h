#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "../External/stb_image.h"


class TextureContainer {
public:

	TextureContainer(std::string texturePath, bool flipVertically);

	void use(unsigned int textureNumber);
	void setImageToClamp(unsigned int textureNumber);
	
	int getID();
private:
	unsigned int ID;
};
