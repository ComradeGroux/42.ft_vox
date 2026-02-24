#include "WorldGenerator.hpp"

WorldGenerator::WorldGenerator(uint64_t seed) : _seed(seed), _noise(seed)
{
}

WorldGenerator::~WorldGenerator(void)
{
}

void	WorldGenerator::generate(Chunk& chunk) const
{
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
			_generateColumn(chunk, x, z, BIOME_DEFAULT);
	}
}

void	WorldGenerator::_generateColumn(Chunk& chunk, int localX, int localZ, const BiomeParams& params) const
{
	int	worldX = chunk.getChunkX() * 16 + localX;
	int	worldZ = chunk.getChunkZ() * 16 + localZ;

	float noise = _noise.fractal2D(
		(float)worldX,
		(float)worldZ,
		params.terrain.octaves,
		params.terrain.frequency,
		params.terrain.amplitude,
		params.terrain.lacunarity,
		params.terrain.persistence
	);

	int	height = TERRAIN_HEIGHT_MIN + (int)((noise + 1.0f) / 2.0f * (TERRAIN_HEIGHT_MAX - TERRAIN_HEIGHT_MIN));

	for (int y = 0; y <= height; y++)
	{
		VoxelType type;

		if (y == height)
			type = params.terrain.ground_type;
		else if (y >= height - params.terrain.ground_depth)
			type = params.terrain.ground_depth_type;
		else
			type = VoxelType::Stone;

		chunk.set(localX, y, localZ, type);
	}
}
