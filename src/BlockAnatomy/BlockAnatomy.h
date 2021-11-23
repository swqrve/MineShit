#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <array>

#include "../Texture/TextureContainer.h"
#include "../Shader/Shader.h"
#include "../Block/Block.h"

class BlockManager;

enum class BlockAttribute {
    INTERACTABLE,
    SOLID,
    TRANSPARENT,
    DROPS_ITEM
};

struct RenderInformation {
public:
    RenderInformation(float incomingVertices[180], int amountOfVerts) {
        this->amountOfVertices = amountOfVerts;
        for (int i = 0; i < amountOfVertices; i++) this->vertices[i] = incomingVertices[i];
    }

    float* getVertices() { return this->vertices; }
    int getAmountOfVertices() { return this->amountOfVertices; }
    int amountOfVertices = 0;

private:
    float vertices[180] = {};
};

class BlockAnatomy {
public:
    BlockAnatomy(int blockID, std::vector<BlockAttribute> blockAttributes, std::string blockName, BlockManager* blockManager, int textureAtlasCoords[6]);
    ~BlockAnatomy();

    RenderInformation render(Block* block, bool renderTopFace, bool renderBottomFace, bool renderSideOneFace, bool renderSideTwoFace, bool renderSideThreeFace, bool renderSideFourFace);

    std::vector<BlockAttribute>& getBlockAttributes();
    std::string& getBlockName();

    BlockManager* getBlockManager();
private:
    float getTextureCoordinates(int side, int position, int valueToReturn, int startingIndex);
    void addSpecificVertices(float* vertices, int& currentArrayIndex, int startingIndex, int endingIndex, glm::ivec3& offSet, bool run, int side);
    void recalculateValues(float& toTheRight, float& down, float& downTwice, float& toTheRightTwice, int side);

    std::vector<BlockAttribute> blockAttributes;
    std::string blockName;
    
    Shader blockShader;

    int blockID;
    BlockManager* blockManager;

    int textureAtlasCoord[6];
    std::unordered_map<int, std::array<float, 12>> textureCoordsMap;

    float vertices[180];
    
    // Later add itemstacks for the drop item
};