#include "ChunkColumn.h"

#include "../World/World.h"
#include "../Chunk/Chunk.h"
#include "../Noise/Noise.h"
#include "../Block/Block.h"
#include "../ChunkLayer/ChunkLayer.h"

ChunkColumn::ChunkColumn(World* world, glm::ivec2 columnCoords, Biome biome, Noise* noise) : world(world), columnCoords(columnCoords), biome(biome), noise(noise) {
	for (int i = 0; i < 16; i++) chunks.push_back(new Chunk(16, 16, 16, world, glm::ivec3(columnCoords.x, i, columnCoords.y)));
}

ChunkColumn::~ChunkColumn() {
	for (Chunk* chunk : chunks)	delete chunk;
}

void ChunkColumn::generate() {
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {			
			int height = world->getNoiseHeight(i + (columnCoords.x * 16), j + (columnCoords.y * 16), noise);
			height += 40;

			int chunkHeight = (int) height / 16;

			Chunk* chunk = nullptr;
			for (Chunk* c : chunks) if (c->getChunkPosition().y == chunkHeight) chunk = c;
			if (chunk == nullptr) continue;

			addBlock(new Block(chunk, 2, glm::ivec3(i, height - (chunkHeight * 16), j)), chunk);
			
			for (int d = height - 1; d > 0; d--) {
				chunkHeight = (int) d / 16;

				Chunk* chunk = nullptr;
				for (Chunk* c : chunks) if (c->getChunkPosition().y == chunkHeight) chunk = c;
				if (chunk == nullptr) continue;

				if (d > (height - 4)) addBlock(new Block(chunk, 1, glm::ivec3(i, d - (chunkHeight * 16), j)), chunk);
				else if (d == 2 || d == 3) {
					if (rand() % 5 > 3) addBlock(new Block(chunk, 3, glm::ivec3(i, d - (chunkHeight * 16), j)), chunk);
					else addBlock(new Block(chunk, 0, glm::ivec3(i, d - (chunkHeight * 16), j)), chunk);
				} else if (d == 1) addBlock(new Block(chunk, 3, glm::ivec3(i, d - (chunkHeight * 16), j)), chunk);
				else addBlock(new Block(chunk, 0, glm::ivec3(i, d - (chunkHeight * 16), j)), chunk);
			}
		}
	}
}

void ChunkColumn::addBlock(Block* block, Chunk* chunk) {
	chunk->getChunkLayer(block->getRelativePosition().y)->getBlocks().push_back(block);
}

glm::ivec2& ChunkColumn::getColumnCoords() { return this->columnCoords; }
std::vector<Chunk*>& ChunkColumn::getChunks() { return this->chunks; }
Biome ChunkColumn::getBiome() { return this->biome; }

