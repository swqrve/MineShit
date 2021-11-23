#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../External/FastNoiseLite.h"
#include "../ChunkLayer/ChunkLayer.h"

#include "Game.h"
#include <thread>

GameState state;
unsigned int width, height;

double lastRan = glfwGetTime();

Game::Game(unsigned int width, unsigned int height) : width(width), height(height), state(GameState::GAME_ACTIVE) { 
     camera = new Camera(glm::vec3(0, 2, 0), 45.0f);
     blockManager = new BlockManager(camera);
     skybox = new Skybox(camera);
     world = new World("Default World", blockManager);

     blockManager->init();
     world->generateWorld();
}

Game::~Game() {
    delete world;
    delete blockManager;
    delete camera;
    delete skybox;
}

// game loop
void Game::update(float dt) {

}

void Game::render() {
    // Enable the texture atlas
    blockManager->getTextureAtlas()->use(GL_TEXTURE0);

    // Render all the chunks blocks
    for (ChunkColumn* column : world->getChunkColumns()) for (Chunk* chunk : column->getChunks()) chunk->render();

    // render the skybox last
    skybox->render();
}

// Handle all types of input
void Game::processMouseMovement(GLFWwindow* window, double xpos, double ypos, float dt) {
    camera->processMouseInput(window, xpos, ypos);
}
void Game::processKeyboardInput(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);
    camera->processKeyboardInput(window, dt);

    if (glfwGetKey(window, GLFW_KEY_M)) {
        static bool wireframe = false;
        glPolygonMode(GL_FRONT_AND_BACK, (wireframe = !wireframe) ? GL_LINE : GL_FILL);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        double currentTime = glfwGetTime();
        if (lastRan + .5 < currentTime) {
            std::cout << "X: " << camera->getCameraPosition().x << " Y: " << camera->getCameraPosition().y << " Z: " << camera->getCameraPosition().z;
            for (ChunkColumn* column : world->getChunkColumns()) for (Chunk* chunk : column->getChunks()) std::cout << "X: " << chunk->getChunkPosition().x << " Y: " << chunk->getChunkPosition().y << " Z: " << chunk->getChunkPosition().z << std::endl;
            lastRan = currentTime;
        }
    }
}

void Game::processScrollInput(GLFWwindow* window, double xoffset, double yoffset, float dt) {
}

void Game::mouseClickInput(GLFWwindow* window, int button, int action, int mods, float dt) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        Raycast raycast(camera);
        
        std::vector<Chunk*> chunksToLoopThrough;
        
        for (ChunkColumn* column : world->getChunkColumns()) for (Chunk* chunk : column->getChunks()) {
            if (glm::distance(camera->getCameraPosition(), glm::vec3(chunk->getChunkPosition() * 16)) > 32) continue;

            chunksToLoopThrough.push_back(chunk);
        }
        
        glm::ivec3 blocksPosition;
        for (Chunk* chunk : chunksToLoopThrough) {
            bool suceeded = false;
            for (ChunkLayer* layer : chunk->getChunkLayers()) {
                for (Block* block : layer->getBlocks()) {
                    int x = block->getRelativePosition().x, y = block->getRelativePosition().y, z = block->getRelativePosition().z;
                    world->convertPositionToWorld(x, y, z, chunk);

                    bool success = false;

                    for (int i = 1; i < 6; i++) {
                        glm::vec3 position = raycast.getEndingPosition((float)i);
                        if (glm::distance(glm::vec3(x, y, z), position) > i) continue; // if the block is greater then the ray cast distance, ignore it 

                        // Check if the point intersects the Block
                        if (!pointIntersects(position, glm::vec3(x, y, z))) continue;
                        
                        blocksPosition = glm::ivec3(block->getRelativePosition().x, block->getRelativePosition().y, block->getRelativePosition().z);
                        layer->getBlocks().erase(std::remove(layer->getBlocks().begin(), layer->getBlocks().end(), block));

                        success = true;
                        break;
                    }

                    if (success) {
                        chunk->setHasUpdate(true);

                        if (blocksPosition.x == 0 || blocksPosition.x == 15) {
                            Chunk* chunkOne = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x + 1, chunk->getChunkPosition().y, chunk->getChunkPosition().z));
                            Chunk* chunkTwo = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x - 1, chunk->getChunkPosition().y, chunk->getChunkPosition().z));

                            if (chunkOne != nullptr) chunkOne->setHasUpdate(true);
                            if (chunkTwo != nullptr) chunkTwo->setHasUpdate(true);
                        }

                        if (blocksPosition.y == 0 || blocksPosition.y == 15) {
                            Chunk* chunkOne = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x, chunk->getChunkPosition().y + 1, chunk->getChunkPosition().z));
                            Chunk* chunkTwo = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x, chunk->getChunkPosition().y - 1, chunk->getChunkPosition().z));

                            if (chunkOne != nullptr) chunkOne->setHasUpdate(true);
                            if (chunkTwo != nullptr) chunkTwo->setHasUpdate(true);
                        }

                        if (blocksPosition.z == 0 || blocksPosition.z == 15) {
                            Chunk* chunkOne = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x, chunk->getChunkPosition().y, chunk->getChunkPosition().z + 1));
                            Chunk* chunkTwo = chunk->getWorld()->getChunk(glm::ivec3(chunk->getChunkPosition().x, chunk->getChunkPosition().y, chunk->getChunkPosition().z - 1));

                            if (chunkOne != nullptr) chunkOne->setHasUpdate(true);
                            if (chunkTwo != nullptr) chunkTwo->setHasUpdate(true);
                        }

                        suceeded = true;
                        break;
                    }
                }
                if (suceeded) break;
            }
            }
    }
}

bool Game::pointIntersects(glm::vec3 position, glm::vec3 cubePosition) {
    return (position.x >= cubePosition.x - 0.5 && position.x <= cubePosition.x + 0.5f &&
            position.y >= cubePosition.y - 0.5f && position.y <= cubePosition.y + 0.5f &&
            position.z >= cubePosition.z - 0.5f && position.z <= cubePosition.z + 0.5f);
}

BlockManager* Game::getBlockManager() { return this->blockManager; }
World* Game::getWorld() { return this->world; }
Camera* Game::getCamera() { return this->camera; }