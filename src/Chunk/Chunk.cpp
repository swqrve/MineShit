#include "Chunk.h"

#include "../ChunkMesh/ChunkMesh.h"
#include "../ChunkLayer/ChunkLayer.h"
#include "../BlockAnatomy/BlockAnatomy.h"
#include "../World/World.h"
#include "../Camera/Camera.h"
#include "../Shader/Shader.h"

Chunk::Chunk(int width, int length, int height, World* world, glm::ivec3 chunkCoords) : width(width), length(length), height(height), world(world), chunkCoords(chunkCoords) {
	mesh = new ChunkMesh();

	for (int i = 0; i < 16; i++) chunkLayers.push_back(new ChunkLayer(this, i));

	mesh->init(this, world->getBlockManager());
}

Chunk::~Chunk() { 
	for (ChunkLayer* layer : chunkLayers) {
		for (Block* block : layer->getBlocks()) delete block;
		delete layer;
	}

	delete mesh;
}

void Chunk::render() {
	double startTime = glfwGetTime();

	BlockManager* manager = world->getBlockManager()->getBlockList().at(0).getBlockManager();
	
	glm::mat4 view = manager->getCamera()->getView();
	glm::mat4 projection = glm::perspective(glm::radians(manager->getCamera()->getFOV()), 1280.0f / 720.0f, 0.1f, 500.0f);

	manager->getShader()->use();

	manager->getShader()->setMat4("view", view);
	manager->getShader()->setMat4("projection", projection);

	mesh->render(hasUpdate, manager->getShader());

	if (hasUpdate) std::cout << (glfwGetTime() - startTime) * 1000 << std::endl;
	hasUpdate = false;
}

ChunkLayer* Chunk::getChunkLayer(unsigned int yPosition) {
	for (ChunkLayer* layer : chunkLayers) if (layer->getYPosition() == yPosition) return layer;
	return nullptr;
}

glm::ivec3& Chunk::getChunkPosition() { return chunkCoords; }

std::vector<ChunkLayer*>& Chunk::getChunkLayers() { return chunkLayers; }

World* Chunk::getWorld() { return world; }

int Chunk::getWidth() { return width; }

int Chunk::getHeight() { return height; }

void Chunk::setHasUpdate(bool value) { this->hasUpdate = value; }