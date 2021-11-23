#pragma once
#include <iostream>
#include <vector>

#include <glm/glm.hpp>

class World;

enum class Biome;

class Chunk;
class Block;
class Noise;

class ChunkColumn {
public:
    ChunkColumn(World* world, glm::ivec2 columnCoords, Biome biome, Noise* noise);
    ~ChunkColumn();

    void generate();

    glm::ivec2& getColumnCoords();
    std::vector<Chunk*>& getChunks();
    Biome getBiome();

    void addBlock(Block* block, Chunk* chunk);
private:
    Biome biome;
    Noise* noise;
    glm::ivec2 columnCoords;

    std::vector<Chunk*> chunks;

    World* world;
};