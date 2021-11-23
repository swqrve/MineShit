#include "ChunkLayer.h"

ChunkLayer::ChunkLayer(Chunk* chunk, unsigned int yPosition) : chunk(chunk), yPosition(yPosition) { }

ChunkLayer::~ChunkLayer() {}

std::vector<Block*>& ChunkLayer::getBlocks() { return this->blocks; }

unsigned int ChunkLayer::getYPosition() { return this->yPosition; }

Chunk* ChunkLayer::getChunk() { return this->chunk; }