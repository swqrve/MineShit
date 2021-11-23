#pragma once

#include <vector>

class Chunk;
class Block;
class BlockManager;
class Shader;
class ChunkLayer;
struct RenderInformation;

class ChunkMesh {
public:
	ChunkMesh();
	~ChunkMesh();

	void render(bool update, Shader* shader);
	void init(Chunk* chunk, BlockManager* manager);
private:
	void generateMesh();
	void updateVBO();

	unsigned int VBO = 0;
	unsigned int VAO = 0;

	Chunk* chunk = nullptr;
	BlockManager* manager = nullptr;

	float vertices[122880] = {};
	int currentAmountOfVertices = 0;


	void calculateFaces(Block* block, bool& faceOne, bool& faceTwo, bool& faceThree, bool& faceFour, bool& topFace, bool& bottomFace, ChunkLayer* layer);
	void addVertices(RenderInformation info);

	std::vector<ChunkLayer*> toLoopThrough;

	int currentBlockAbsoluteX = 0, currentBlockAbsoluteY = 0, currentBlockAbsoluteZ = 0;
	int newBlockX = 0, newBlockY = 0, newBlockZ = 0;
};