#pragma once

#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

#include "../External/FastNoiseLite.h"

class Noise {
public:
    Noise(fnl_noise_type noiseType, fnl_fractal_type fractalType, float frequency, float octaves, float lacunarity, float gain, float weightedStrength, float seed);

    float getValue(int x, int y);
private:
    fnl_state noise = fnlCreateState();
    //std::unordered_map<glm::vec2, float> heightMap;  Later add it so that whenver it gets called it adds the values to a list, so if it ever needs to access that position again it just recalculates it
};