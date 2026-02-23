#pragma once
#include <cstdint>

constexpr int	TERRAIN_HEIGHT_MIN = 50;
constexpr int	TERRAIN_HEIGHT_MAX = 150;

constexpr int	DIRT_LAYER_DEPTH = 5;

constexpr int	RENDER_DISTANCE = 11;

constexpr float	FOV = 80.0f;
constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE  = 320.0f;



/****************/
/***  BIOMES  ***/
/****************/
struct NoiseParams {
	int		octaves;
	float	frequency;
	float	amplitude;
	float	lacunarity;
	float	persistence;
};

constexpr NoiseParams BIOME_DEFAULT = {
	6,		// octaves
	0.003f,	// frequency
	1.0f,	// amplitude
	2.0f,	// lacunarity
	0.5f	// persistence
};
