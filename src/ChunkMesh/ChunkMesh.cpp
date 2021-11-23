#include "ChunkMesh.h"

#include "../Chunk/Chunk.h"
#include "../BlockManager/BlockManager.h"
#include "../World/World.h"
#include "../ChunkLayer/ChunkLayer.h"

ChunkMesh::ChunkMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}


void ChunkMesh::init(Chunk* chunk, BlockManager* manager) {
	this->chunk = chunk;
	this->manager = manager;
}

ChunkMesh::~ChunkMesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void ChunkMesh::render(bool update, Shader* shader) {
	glBindVertexArray(VAO);

	if (update) generateMesh();

	glm::mat4 m = glm::mat4(1.0f);
	m = glm::translate(m, glm::vec3(0.0f, 0.0f, 0.0f));
	shader->setMat4("model", m);
	
	glDrawArrays(GL_TRIANGLES, 0, currentAmountOfVertices);
}

void ChunkMesh::updateVBO() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
}

void ChunkMesh::generateMesh() {
	currentAmountOfVertices = 0;
	BlockManager blockManager = *manager; 

	bool faceOne = false, faceTwo = false, faceThree = false, faceFour = false, topFace = false, bottomFace = false;
	
	for (ChunkLayer* layer : chunk->getChunkLayers()) for (Block* block : layer->getBlocks()) {
		calculateFaces(block, faceOne, faceTwo, faceThree, faceFour, topFace, bottomFace, layer);
	                     	// 0,0,-1|  0,0,1  | -1,0,0 |   1, 0, 0 | 0, 1, 0 | 0, -1, 0
		addVertices(blockManager.getBlockList().at(block->getBlockAnatomyID()).render(block, bottomFace, topFace, faceOne, faceTwo, faceThree, faceFour));
	}	

	updateVBO();
}

void ChunkMesh::calculateFaces(Block* block, bool& faceOne, bool& faceTwo, bool& faceThree, bool& faceFour, bool& topFace, bool& bottomFace, ChunkLayer* layer) {
	faceOne = true, faceTwo = true, faceThree = true, faceFour = true, topFace = true, bottomFace = true;

	if (block->getRelativePosition().x == 0 || block->getRelativePosition().x == 15) {
		Chunk* chunkOne = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x + 1, chunk->getChunkPosition().y, chunk->getChunkPosition().z));
		Chunk* chunkTwo = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x - 1, chunk->getChunkPosition().y, chunk->getChunkPosition().z));
	
		if (chunkOne != nullptr) toLoopThrough.push_back(chunkOne->getChunkLayer(block->getRelativePosition().y));
		if (chunkTwo != nullptr) toLoopThrough.push_back(chunkTwo->getChunkLayer(block->getRelativePosition().y));
	}
	
	if (block->getRelativePosition().y == 0 || block->getRelativePosition().y == 15) {
		Chunk* chunkOne = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x, chunk->getChunkPosition().y + 1, chunk->getChunkPosition().z));
		Chunk* chunkTwo = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x, chunk->getChunkPosition().y - 1, chunk->getChunkPosition().z));
	
		if (chunkOne != nullptr) toLoopThrough.push_back(chunkOne->getChunkLayer(0));
		if (chunkTwo != nullptr) toLoopThrough.push_back(chunkTwo->getChunkLayer(15));
	}
	
	if (block->getRelativePosition().z == 0 || block->getRelativePosition().z == 15) {
		Chunk* chunkOne = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x, chunk->getChunkPosition().y, chunk->getChunkPosition().z + 1));
		Chunk* chunkTwo = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x, chunk->getChunkPosition().y, chunk->getChunkPosition().z - 1));
	
		if (chunkOne != nullptr) toLoopThrough.push_back(chunkOne->getChunkLayer(block->getRelativePosition().y));
		if (chunkTwo != nullptr) toLoopThrough.push_back(chunkTwo->getChunkLayer(block->getRelativePosition().y));
	}

	if (layer->getYPosition() != 15) toLoopThrough.push_back(chunk->getChunkLayer(layer->getYPosition() + 1));
	if (layer->getYPosition() != 0) toLoopThrough.push_back(chunk->getChunkLayer(layer->getYPosition() - 1));
	
	toLoopThrough.push_back(layer);

	currentBlockAbsoluteX = block->getRelativePosition().x + (chunk->getChunkPosition().x * 16);
	currentBlockAbsoluteY = block->getRelativePosition().y + (chunk->getChunkPosition().y * 16);
	currentBlockAbsoluteZ = block->getRelativePosition().z + (chunk->getChunkPosition().z * 16);

	for (ChunkLayer* l : toLoopThrough) {
		auto chunkRelativePosition = l->getChunk()->getChunkPosition();
		for (Block* b : l->getBlocks()) {
			auto blockRelativePosition = b->getRelativePosition();

			newBlockX = blockRelativePosition.x + (chunkRelativePosition.x * 16);
			newBlockY = blockRelativePosition.y + (chunkRelativePosition.y * 16);
			newBlockZ = blockRelativePosition.z + (chunkRelativePosition.z * 16);


			if (newBlockY == currentBlockAbsoluteY) {
				if ((faceTwo || faceOne) && newBlockX == currentBlockAbsoluteX) {
					if (faceOne && newBlockZ + 1 == currentBlockAbsoluteZ) faceOne = false;
					if (faceTwo && newBlockZ - 1 == currentBlockAbsoluteZ) faceTwo = false;
				} else if ((faceFour || faceThree) && newBlockZ == currentBlockAbsoluteZ) {
					if (faceThree && newBlockX + 1 == currentBlockAbsoluteX) faceThree = false;
					if (faceFour && newBlockX - 1 == currentBlockAbsoluteX) faceFour = false;
				}
			} else if (topFace || bottomFace) {
				if (newBlockX == currentBlockAbsoluteX && newBlockZ == currentBlockAbsoluteZ) {
					if (topFace && newBlockY + 1 == currentBlockAbsoluteY) topFace = false;
					if (bottomFace && newBlockY - 1 == currentBlockAbsoluteY) bottomFace = false;
				}
			}
		}
	}

	toLoopThrough.clear();
}

void ChunkMesh::addVertices(RenderInformation info) {
	for (int i = 0; i < info.getAmountOfVertices(); i++) vertices[currentAmountOfVertices++] = info.getVertices()[i];
}

