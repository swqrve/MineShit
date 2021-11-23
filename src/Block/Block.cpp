#include "Block.h"

Block::Block(Chunk* chunk, int blockAnatomyID, glm::ivec3 relativePosition) : currentChunk(chunk), blockAnatomyID(blockAnatomyID), relativePosition(relativePosition) { }

Block::~Block() { }

Chunk* Block::getChunk() { return currentChunk; }

glm::ivec3& Block::getRelativePosition() { return relativePosition; }

void Block::setBlock(int blockAnatomyID) { this->blockAnatomyID = blockAnatomyID; }

int Block::getBlockAnatomyID() { return blockAnatomyID; }
