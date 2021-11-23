#include "Noise.h"

Noise::Noise(fnl_noise_type noiseType, fnl_fractal_type fractalType, float frequency, float octaves, float lacunarity, float gain, float weightedStrength, float seed) {
	noise.noise_type = noiseType;
	noise.fractal_type = fractalType;
	
	if (fractalType == FNL_FRACTAL_NONE) return;

	noise.frequency = frequency;
	noise.octaves = octaves;
	noise.lacunarity = lacunarity;
	noise.gain = gain;
	noise.weighted_strength = weightedStrength;

	noise.seed = (int) seed;
} 

float Noise::getValue(int x, int y) {
	return fnlGetNoise2D(&noise, x, y);
}
