#pragma once

#include <GLFW/glfw3.h>
#include <cstdint>
#include "Section.hpp"

/********************/
/*****  RENDER  *****/
/********************/
constexpr int	RENDER_DISTANCE = 11;

constexpr float	FOV = 80.0f;
constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE  = (RENDER_DISTANCE * 1.5) * 16;

constexpr GLsizei	TEXTURE_SIZE = 16;
constexpr GLsizei	ATLAS_SIZE = 256;

constexpr float	DAY_SPEED = 0.1f;


/********************/
/*****  INPUTS  *****/
/********************/
constexpr int KEY_FORWARD   = GLFW_KEY_W;
constexpr int KEY_BACKWARD  = GLFW_KEY_S;
constexpr int KEY_LEFT      = GLFW_KEY_A;
constexpr int KEY_RIGHT     = GLFW_KEY_D;
constexpr int KEY_UP        = GLFW_KEY_SPACE;
constexpr int KEY_DOWN      = GLFW_KEY_LEFT_CONTROL;
constexpr int KEY_SPRINT    = GLFW_KEY_LEFT_SHIFT;
constexpr int KEY_QUIT      = GLFW_KEY_ESCAPE;

constexpr float PLAYER_SPEED = 1.0f;
constexpr float PLAYER_RUNNING_SPEED = 20.0f;
constexpr float MOUSE_SENSITIVITY = 0.1f;




/********************/
/*****  BIOMES  *****/
/********************/

constexpr int	BLEND_RADIUS = 16;
constexpr int	GRID_SIZE = 16 + 2 * BLEND_RADIUS;
constexpr int	SEA_LEVEL = 64;

struct TerrainParams {
	int			octaves;
	float		frequency;
	float		amplitude;
	float		lacunarity;
	float		persistence;

	int			terrain_min_height;
	int			terrain_max_height;
	VoxelType	ground_type;		// Type de bloc de la surface
	VoxelType	ground_depth_type;	// Type de bloc du layer juste sous la surface
	int			ground_depth;		// Nombre de bloc que fait le layer juste sous la surface
};

struct CaveParams {
	int		octaves;
	float	frequency;
	float	frequencyY;
	float	amplitude;
	float	lacunarity;
	float	persistence;

	float	threshold;
	float	surfaceRatio;
};

struct BiomeParams {
	float			temperature;
	float			humidity;
	bool			isOcean;
	TerrainParams	terrain;
	CaveParams		cave;
};

constexpr BiomeParams BIOME_PLAIN = {
	0.5f,
	0.6f,
	false,
	{
		6, 0.003f, 1.0f, 2.0f, 0.5f,
		65, 100,
		VoxelType::Grass, VoxelType::Dirt, 5
	},
	{
		3, 0.08f, 0.1f, 1.0f, 2.0f, 0.5f,
		0.4f, 0.8f
	}
};

constexpr BiomeParams BIOME_MOUNTAIN = {
	0.3f,
	0.4f,
	false,
	{
		8, 0.005f, 1.0f, 2.0f, 0.6f,
		100, 220,
		VoxelType::Stone, VoxelType::Stone, 3
	},
	{
		3, 0.08f, 0.1f, 1.0f, 2.0f, 0.5f,
		0.4f, 0.8f
	}
};

constexpr BiomeParams BIOME_DESERT = {
	0.9f,
	0.1f,
	false,
	{
		4, 0.002f, 1.0f, 2.0f, 0.4f,
		65, 85,
		VoxelType::Sand, VoxelType::Sand, 8
	},
	{
		3, 0.08f, 0.1f, 1.0f, 2.0f, 0.5f,
		0.4f, 0.8f
	}
};

constexpr BiomeParams BIOME_OCEAN = {
	0.5f,
	1.0f,
	true,
	{
		1, 0.001f, 1.0f, 2.0f, 0.4f,
		50, 64,
		VoxelType::Sand, VoxelType::Stone, 3
	},
	{
		3, 0.08f, 0.1f, 1.0f, 2.0f, 0.5f,
		0.4f, 0.8f
	}
};

constexpr int	NUMBER_BIOME = 4;
constexpr std::array<BiomeParams, NUMBER_BIOME> BIOMES = {
	BIOME_PLAIN,
	BIOME_MOUNTAIN,
	BIOME_DESERT,
	BIOME_OCEAN
};
