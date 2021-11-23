#pragma once

#include <glm/glm.hpp>

class Chunk;

class Block {
public:
    Block(Chunk* chunk, int blockAnatomyID, glm::ivec3 relativePosition);
    ~Block();

    Chunk* getChunk();
    glm::ivec3& getRelativePosition();
    
    void setBlock(int blockAnatomyID);
    int getBlockAnatomyID();
private:
    int blockAnatomyID;

    glm::ivec3 relativePosition;

    Chunk* currentChunk;
};