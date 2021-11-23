#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include "../Texture/TextureContainer.h"
#include "../Shader/Shader.h"
#include "../BlockAnatomy/BlockAnatomy.h"

class Camera;

class BlockManager {
public:
	BlockManager(Camera* camera);
	~BlockManager(); 

	void init();
	
	std::unordered_map<int, BlockAnatomy>& getBlockList();

	Camera* getCamera();
	Shader* getShader();
	TextureContainer* getTextureAtlas();

	float* getCubeVertices();

private:
	std::string getJSONFromPath(std::string path);

	std::unordered_map<int, BlockAnatomy> blockList;

	Shader* blockShader;
	Camera* camera;

	TextureContainer* textureAtlas;

	float vertices[180] = {};
};