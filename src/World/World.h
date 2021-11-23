#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include "../ChunkColumn/ChunkColumn.h"
#include "../Chunk/Chunk.h"
#include "../BlockManager/BlockManager.h"
#include "../Noise/Noise.h"

enum class Biome {
    PLAINS, FOREST, MOUNTAINS, DESERT
};

class World {
public:
    World(std::string name, BlockManager* manager);
    ~World();

    std::vector<ChunkColumn*>& getChunkColumns();

    Chunk* getChunk(glm::ivec3 chunkCoords);
    BlockManager* getBlockManager();

    void generateWorld();
    void convertPositionToWorld(int& x, int& y, int& z, Chunk* c);
    int getNoiseHeight(int x, int y, Noise* noise);
private:
    std::string worldName;

    BlockManager* manager;
    std::vector<ChunkColumn*> chunkColumns;

    Noise* biomeNoise;

    Noise* mountainNoise;
    Noise* plainNoise;
    Noise* forestNoise;


    int chunkID = 0;
};