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
/***  GENERATION  ***/
/********************/
constexpr int	TERRAIN_HEIGHT_MIN = 50;
constexpr int	TERRAIN_HEIGHT_MAX = 150;



/********************/
/*****  BIOMES  *****/
/********************/

struct TerrainParams {
	int			octaves;
	float		frequency;
	float		amplitude;
	float		lacunarity;
	float		persistence;

	int				ground_depth;		// Nombre de bloc que fait le layer juste sous la surface
	VoxelType		ground_depth_type;	// Type de bloc du layer juste sous la surface
	VoxelType		ground_type;		// Type de bloc de la surface
};

struct CaveParams {
	float	frequency;
	float	threshold;
	int		maxHeight;
};

struct BiomeParams {
	TerrainParams	terrain;
	CaveParams		cave;
};

constexpr BiomeParams BIOME_DEFAULT = {
	{
		6,		// octaves
		0.003f,	// frequency
		1.0f,	// amplitude
		2.0f,	// lacunarity
		0.5f,	// persistence

		3,
		VoxelType::Dirt,
		VoxelType::Grass
	},
	{

	}
};

constexpr BiomeParams BIOME_DESERT = {
	{
		3,		// octaves
		0.02f,  // frequency
		0.005f,	// amplitude
		1.8f,	// lacunarity
		0.5f,	// persistence

		15,
		VoxelType::Sand,
		VoxelType::Sand
	},
	{

	}
};
