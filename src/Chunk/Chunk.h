#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "../Block/Block.h"

class ChunkLayer;
class World;
class ChunkMesh;

class Chunk {
public:
    Chunk(int width, int length, int height, World* world, glm::ivec3 chunkCoords);
    ~Chunk();

    glm::ivec3& getChunkPosition();

    void render();

    std::vector<ChunkLayer*>& getChunkLayers();

    ChunkLayer *getChunkLayer(unsigned int yPosition);
    World *getWorld();
    
    int getWidth();
    int getHeight();

    void setHasUpdate(bool value);
private:
    glm::ivec3 chunkCoords;

    int width;
    int length;
    int height;

    std::vector<ChunkLayer*> chunkLayers;

    ChunkMesh* mesh;
    World* world;
    
    bool hasUpdate = true;
};