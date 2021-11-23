#include "BlockManager.h"

#include <filesystem>
#include "rapidjson/document.h"
#include "../Camera/Camera.h"


BlockManager::BlockManager(Camera* camera) : camera(camera) { }

BlockManager::~BlockManager() {
   // delete blockShader;
   // delete textureAtlas; // They generate exceptions if you delete them?
}

void BlockManager::init() {
    // Load Block Shader
    blockShader = new Shader("resources/shaders/BlockShader/BlockShader.glsl", "resources/shaders/BlockShader/BlockFragment.glsl");

    //Load Texture Atlas
    textureAtlas = new TextureContainer("resources/textures/atlas.png", false);
   
    // Load every block from JSON
    const std::filesystem::path blockDirectory{ "resources/blocks" };

    for (auto const& systemFile : std::filesystem::directory_iterator{ blockDirectory }) {

        rapidjson::Document document;
        document.Parse(getJSONFromPath(std::filesystem::absolute(systemFile.path()).string()).c_str());

        int blockID = document["Block_ID"].GetInt();

        std::vector<BlockAttribute> attributes;

        if (document["Attributes"].GetObject()["Interactable"].GetBool()) attributes.push_back(BlockAttribute::INTERACTABLE);
        if (document["Attributes"].GetObject()["Solid"].GetBool()) attributes.push_back(BlockAttribute::SOLID);
        if (document["Attributes"].GetObject()["Transparent"].GetBool()) attributes.push_back(BlockAttribute::TRANSPARENT);
        if (document["Attributes"].GetObject()["DropsItem"].GetBool()) attributes.push_back(BlockAttribute::DROPS_ITEM);

        int textureCoordsIndexList[6] {};
        
        for (int i = 0; i < 6; i++) textureCoordsIndexList[i] = document[("TextureCoord" + std::to_string(i + 1)).c_str()].GetInt();
        
        std::string name = document["Name"].GetString();
        
        blockList.insert({ blockID, BlockAnatomy(blockID, attributes, name, this, textureCoordsIndexList) });
    }

    float cubeVertices[] = {
        //Side One
        -0.5f, -0.5f, -0.5f, 0.1f,  0.1f,
         0.5f,  0.5f, -0.5f, 0.2f,  0.0f,
         0.5f, -0.5f, -0.5f, 0.2f,  0.1f,
         0.5f,  0.5f, -0.5f, 0.2f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.1f,  0.1f,
        -0.5f,  0.5f, -0.5f, 0.1f,  0.0f,

        //Side Two
        -0.5f, -0.5f,  0.5f, 0.1f,  0.1f,
         0.5f, -0.5f,  0.5f, 0.2f,  0.1f,
         0.5f,  0.5f,  0.5f, 0.2f,  0.0f,
         0.5f,  0.5f,  0.5f, 0.2f,  0.0f,
        -0.5f,  0.5f,  0.5f, 0.1f,  0.0f,
        -0.5f, -0.5f,  0.5f, 0.1f,  0.1f,

        //Side Three 
        -0.5f,  0.5f,  0.5f, 0.1f,  0.0f,
        -0.5f,  0.5f, -0.5f, 0.2f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.2f,  0.1f,
        -0.5f, -0.5f, -0.5f, 0.2f,  0.1f,
        -0.5f, -0.5f,  0.5f, 0.1f,  0.1f,
        -0.5f,  0.5f,  0.5f, 0.1f,  0.0f,

        //Side Four
         0.5f,  0.5f,  0.5f, 0.1f,  0.0f,
         0.5f, -0.5f, -0.5f, 0.2f,  0.1f,
         0.5f,  0.5f, -0.5f, 0.2f,  0.0f,
         0.5f, -0.5f, -0.5f, 0.2f,  0.1f,
         0.5f,  0.5f,  0.5f, 0.1f,  0.0f,
         0.5f, -0.5f,  0.5f, 0.1f,  0.1f,

        // Bottom
        -0.5f, -0.5f, -0.5f, 0.0f, 0.1f, 
         0.5f, -0.5f, -0.5f, 0.1f, 0.1f, 
         0.5f, -0.5f,  0.5f, 0.1f, 0.0f, 
         0.5f, -0.5f,  0.5f, 0.1f, 0.0f, 
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 
        -0.5f, -0.5f, -0.5f, 0.0f, 0.1f, 

        // Top
        -0.5f,  0.5f, -0.5f, 0.2f, 0.0f,  
         0.5f,  0.5f,  0.5f, 0.3f, 0.1f, 
         0.5f,  0.5f, -0.5f, 0.3f, 0.0f, 
         0.5f,  0.5f,  0.5f, 0.3f, 0.1f, 
        -0.5f,  0.5f, -0.5f, 0.2f, 0.0f, 
        -0.5f,  0.5f,  0.5f, 0.2f, 0.1f 
    };

    int curentIndex = -1;
    for (int i = 0; i < 180; i++) vertices[++curentIndex] = cubeVertices[i];
}

std::string BlockManager::getJSONFromPath(std::string path) {
    std::string jsonCode;

    std::ifstream jsonCodeFile;

    jsonCodeFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        jsonCodeFile.open(path);
        std::stringstream jsonCodeStream;

        jsonCodeStream << jsonCodeFile.rdbuf();
        jsonCodeFile.close();

        jsonCode = jsonCodeStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    return jsonCode;
}


float* BlockManager::getCubeVertices() { return this->vertices; }

std::unordered_map<int, BlockAnatomy>& BlockManager::getBlockList() { return blockList; }

Camera* BlockManager::getCamera() { return this->camera; }

Shader* BlockManager::getShader() { return this->blockShader; }

TextureContainer* BlockManager::getTextureAtlas() { return this->textureAtlas; }