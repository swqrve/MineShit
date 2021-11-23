#include "BlockAnatomy.h"

#include "../Chunk/Chunk.h"
#include "../BlockManager/BlockManager.h"
#include "../Camera/Camera.h"
#include <array>

BlockAnatomy::BlockAnatomy(int blockID, std::vector<BlockAttribute> blockAttributes, std::string blockName, BlockManager* blockManager, int textureAtlasCoords[6]) : blockID(blockID), blockAttributes(blockAttributes), blockName(blockName), blockShader(blockShader), blockManager(blockManager) {
    for (int i = 0; i < 6; i++)  this->textureAtlasCoord[i] = textureAtlasCoords[i];

    // Generate the appropriate texture coordinates for each side

    float toTheRight = 0.0f;
    float down = 0.0f;
    float downTwice = 0.0f;
    float toTheRightTwice = 0.0f;

    {
        recalculateValues(toTheRight, down, downTwice, toTheRightTwice, 0);
        float sideOneTextureCoords[12]{
        toTheRight, downTwice,
        toTheRightTwice, down,
        toTheRightTwice, downTwice, 
        toTheRightTwice, down, 
        toTheRight, downTwice,
        toTheRight, down
    };

        std::array<float, 12> a;
        std::copy(sideOneTextureCoords, sideOneTextureCoords + 12, begin(a));

        textureCoordsMap.insert({ 1, a });
    }

    {
        recalculateValues(toTheRight, down, downTwice, toTheRightTwice, 1);
        float sideTwoTextureCoords[12] {
        toTheRight, downTwice,
        toTheRightTwice, downTwice,
        toTheRightTwice, down,
        toTheRightTwice, down, 
        toTheRight, down,
        toTheRight, downTwice
    };

        std::array<float, 12> a;
        std::copy(sideTwoTextureCoords, sideTwoTextureCoords + 12, begin(a));

        textureCoordsMap.insert({ 2, a });
    }

    {
        recalculateValues(toTheRight, down, downTwice, toTheRightTwice, 2);
        float sideThreeTextureCoords[12]{
            toTheRight, down,
            toTheRightTwice, down,
            toTheRightTwice, downTwice,
            toTheRightTwice, downTwice,
            toTheRight, downTwice,
            toTheRight, down
        };

        std::array<float, 12> a;
        std::copy(sideThreeTextureCoords, sideThreeTextureCoords + 12, begin(a));

        textureCoordsMap.insert({ 3, a });
    }
    
    {
        recalculateValues(toTheRight, down, downTwice, toTheRightTwice, 3);
        float sideFourTextureCoords[12]{
            toTheRight, down,
            toTheRightTwice, downTwice,
            toTheRightTwice, down,
            toTheRightTwice, downTwice,
            toTheRight, down,
            toTheRight, downTwice,
        };

        std::array<float, 12> a;
        std::copy(sideFourTextureCoords, sideFourTextureCoords + 12, begin(a));

        textureCoordsMap.insert({ 4, a });
    }
    
    {
        recalculateValues(toTheRight, down, downTwice, toTheRightTwice, 5);
        float bottomTextureCoords[12]{
            toTheRight, downTwice,
            toTheRightTwice, downTwice,
            toTheRightTwice, down,
            toTheRightTwice, down,
            toTheRight, down,
            toTheRight, downTwice,
        };

        std::array<float, 12> a;
        std::copy(bottomTextureCoords, bottomTextureCoords + 12, begin(a));

        textureCoordsMap.insert({ 5, a });
    }
    
    {
        recalculateValues(toTheRight, down, downTwice, toTheRightTwice, 4);
        float topTextureCoords[12]{
            toTheRight, down,
            toTheRightTwice, downTwice,
            toTheRightTwice, down,
            toTheRightTwice, downTwice,
            toTheRight, down,
            toTheRight, downTwice
        };

        std::array<float, 12> a;
        std::copy(topTextureCoords, topTextureCoords + 12, begin(a));

        textureCoordsMap.insert({ 6, a });
    }
}

BlockAnatomy::~BlockAnatomy() {

}

RenderInformation BlockAnatomy::render(Block* block, bool renderTopFace, bool renderBottomFace, bool renderSideOneFace, bool renderSideTwoFace, bool renderSideThreeFace, bool renderSideFourFace) {
	// DO ALL THE GENERATING VERTICES CODE
    int amountOfVertices = 0;

    glm::ivec3 positionOffSet((block->getRelativePosition() + (block->getChunk()->getChunkPosition() * 16)));

    // Add the cube faces to the list that need to be rendered :D

    addSpecificVertices(vertices, amountOfVertices, 0, 30, positionOffSet, renderSideOneFace, 1);
    addSpecificVertices(vertices, amountOfVertices, 30, 60, positionOffSet, renderSideTwoFace, 2);
    addSpecificVertices(vertices, amountOfVertices, 60, 90, positionOffSet, renderSideThreeFace, 3);
    addSpecificVertices(vertices, amountOfVertices, 90, 120, positionOffSet, renderSideFourFace, 4);
    addSpecificVertices(vertices, amountOfVertices, 120, 150, positionOffSet, renderBottomFace, 5);
    addSpecificVertices(vertices, amountOfVertices, 150, 180, positionOffSet, renderTopFace, 6);

    return RenderInformation(vertices, amountOfVertices);
}

void BlockAnatomy::addSpecificVertices(float* vertices, int& amountOfVertices, int startingIndex, int endingIndex, glm::ivec3& offSet, bool run, int side) {
    if (!run) return;

    for (int i = startingIndex; i < endingIndex; i += 5) {
        for (int j = 0; j < 5; j++) {
            if (j == 0) vertices[amountOfVertices++] = blockManager->getCubeVertices()[i + j] + offSet.x;
            else if (j == 1) vertices[amountOfVertices++] = blockManager->getCubeVertices()[i + j] + offSet.y;
            else if (j == 2) vertices[amountOfVertices++] = blockManager->getCubeVertices()[i + j] + offSet.z;
            else if (j == 3) vertices[amountOfVertices++] = getTextureCoordinates(side, i, 0, startingIndex);
            else vertices[amountOfVertices++] = getTextureCoordinates(side, i, 1, startingIndex);
        }
    }
}


float BlockAnatomy::getTextureCoordinates(int side, int position, int valueToReturn, int startingIndex) {   
    position -= startingIndex;

    switch (position) {
        case 0: if (valueToReturn == 0) return textureCoordsMap.at(side)[0]; else return textureCoordsMap.at(side)[1]; break;
        case 5: if (valueToReturn == 0) return textureCoordsMap.at(side)[2]; else return textureCoordsMap.at(side)[3]; break;
        case 10: if (valueToReturn == 0) return textureCoordsMap.at(side)[4]; else return textureCoordsMap.at(side)[5]; break;
        case 15: if (valueToReturn == 0) return textureCoordsMap.at(side)[6]; else return textureCoordsMap.at(side)[7]; break;
        case 20: if (valueToReturn == 0) return textureCoordsMap.at(side)[8]; else return textureCoordsMap.at(side)[9]; break;
        case 25: if (valueToReturn == 0) return textureCoordsMap.at(side)[10]; else return textureCoordsMap.at(side)[11]; break;
    }

    return 0;
}

void BlockAnatomy::recalculateValues(float& toTheRight, float& down, float& downTwice, float& toTheRightTwice, int side) {
    toTheRight = (float) textureAtlasCoord[side] / 10.0f;
    down = 0.0f;
    downTwice = .1f;
    toTheRightTwice = toTheRight + .1f;

    if (textureAtlasCoord[side] > 10) {
        toTheRight = (float)(textureAtlasCoord[0] % 10);
        toTheRight = toTheRight / 10.0f;
        toTheRightTwice = toTheRight + .1f;
        down = (textureAtlasCoord[0] - toTheRight) / 10.0f;
        downTwice = down + .1f;
    }
}

std::vector<BlockAttribute>& BlockAnatomy::getBlockAttributes() { return this->blockAttributes; }

std::string& BlockAnatomy::getBlockName() { return this->blockName; }

BlockManager* BlockAnatomy::getBlockManager() { return this->blockManager; }
