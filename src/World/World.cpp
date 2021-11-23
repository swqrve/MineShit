#include "World.h"

#include "../Block/Block.h"
#include "../Chunk/Chunk.h"

World::World(std::string name, BlockManager* manager) : worldName(name) {
	this->manager = manager;
	
	// Prepare the noise and give it a random seed.
	srand((unsigned int) time(NULL));
	float seed = (float) (rand() % 200);
	seed *= 1000 + (rand() % 15);

	std::cout << seed << std::endl;

	// Create all the noises and set their settings
	biomeNoise = new Noise(FNL_NOISE_OPENSIMPLEX2S, FNL_FRACTAL_NONE, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, seed);

    mountainNoise = new Noise(FNL_NOISE_CELLULAR, FNL_FRACTAL_FBM, 0.009f, 12.0f, 1.50f, -0.60f, 0.10f, seed);
	plainNoise = new Noise(FNL_NOISE_PERLIN, FNL_FRACTAL_FBM, 0.002f, 3.0f, 2.00f, 0.60f, 0.10f, seed);
	forestNoise = new Noise(FNL_NOISE_PERLIN, FNL_FRACTAL_FBM, 0.001f, 7.0f, 1.50f, 0.60f, -0.50f, seed);
}

World::~World() {
	for (ChunkColumn* column : chunkColumns) delete column;

	delete mountainNoise;
	delete plainNoise;
	delete forestNoise;
}

std::vector<ChunkColumn*>& World::getChunkColumns() { return chunkColumns; }
BlockManager* World::getBlockManager() { return manager; }


Chunk* World::getChunk(glm::ivec3 chunkCoords) {
	for (ChunkColumn* chunkColumn : chunkColumns) for (Chunk* chunk : chunkColumn->getChunks()) if (chunk->getChunkPosition().x == chunkCoords.x && chunk->getChunkPosition().y == chunkCoords.y && chunk->getChunkPosition().z == chunkCoords.z) return chunk;
	return nullptr;
}

void World::generateWorld() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			ChunkColumn* column = new ChunkColumn(this, glm::ivec2(i, j), Biome::MOUNTAINS, mountainNoise);
			
			column->generate();
			chunkColumns.push_back(column);
		}
	}
}

int World::getNoiseHeight(int x, int y, Noise* noise) {
	float height = (noise->getValue(x, y) * 256) / 2;

	return (int) abs(height);
}


void World::convertPositionToWorld(int& x, int& y, int& z, Chunk* c) {
	x += c->getChunkPosition().x * 16;
	y += c->getChunkPosition().y * 16;
	z += c->getChunkPosition().z * 16;
}