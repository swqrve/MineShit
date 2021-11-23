#pragma once
#include <iostream>
#include <vector>

class Shader;
class Camera;

class Skybox {
public:
    Skybox(Camera* camera);
    ~Skybox();

    void render();
private:
    void loadCubemap(std::vector<std::string> faces);

    std::vector<std::string> faces;

    Shader* skyboxShader = nullptr;
    Camera* camera = nullptr;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int skyBoxTextureID;
};