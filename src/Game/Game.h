#pragma once

#include "../BlockManager/BlockManager.h"
#include "../World/World.h"
#include "../Camera/Camera.h"
#include "../SkyBox/Skybox.h"
#include "../Raycast/Raycast.h"

enum class GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {
public:
    GameState state;
    unsigned int width, height;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void processMouseMovement(GLFWwindow* window, double xpos, double ypos, float dt);
    void processKeyboardInput(GLFWwindow* window, float dt);
    void processScrollInput(GLFWwindow* window, double xoffset, double yoffset, float dt);
    void mouseClickInput(GLFWwindow* window, int button, int action, int mods, float dt);

    void update(float dt);
    void render();

    BlockManager* getBlockManager();
    World* getWorld();
    Camera* getCamera();

    bool pointIntersects(glm::vec3 position, glm::vec3 cubePosition);
private:
    BlockManager* blockManager;
    World* world;
    Camera* camera;
    Skybox* skybox;
};

