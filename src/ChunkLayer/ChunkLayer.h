#pragma once

#include <vector>

class Block;
class Chunk;

class ChunkLayer {
public:
	ChunkLayer(Chunk* chunk, unsigned int yPosition);
	~ChunkLayer();

	std::vector<Block*>& getBlocks();
	unsigned int getYPosition();
	Chunk* getChunk();

private:
	unsigned int yPosition = 0;

	Chunk* chunk = nullptr;

	std::vector<Block*> blocks;
};