#include "WorldGenerator.hpp"

#include "glm.hpp"

WorldGenerator::WorldGenerator(uint64_t seed) : _seed(seed), _noise(seed)
{
}

WorldGenerator::~WorldGenerator(void)
{
}

void	WorldGenerator::generate(Chunk& chunk) const
{
	int	y;
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			y = _generateColumn(chunk, x, z, BIOME_DEFAULT.terrain);
			_generateCaves(chunk, x, y, z, BIOME_DEFAULT.cave);
		}
	}
}

int	WorldGenerator::_generateColumn(Chunk& chunk, int localX, int localZ, const TerrainParams& params) const
{
	int	worldX = chunk.getChunkX() * 16 + localX;
	int	worldZ = chunk.getChunkZ() * 16 + localZ;

	float noise = _noise.fractal2D(
		(float)worldX,
		(float)worldZ,
		params.octaves,
		params.frequency,
		params.amplitude,
		params.lacunarity,
		params.persistence
	);

	int	height = TERRAIN_HEIGHT_MIN + (int)((noise + 1.0f) / 2.0f * (TERRAIN_HEIGHT_MAX - TERRAIN_HEIGHT_MIN));

	for (int y = 0; y <= height; y++)
	{
		VoxelType type;

		if (y == height)
			type = params.ground_type;
		else if (y >= height - params.ground_depth)
			type = params.ground_depth_type;
		else
			type = VoxelType::Stone;

		chunk.set(localX, y, localZ, type);
	}
	return height;
}

void	WorldGenerator::_generateCaves(Chunk& chunk, int localX, int maxY, int localZ, const CaveParams& params) const
{
	int worldX = chunk.getChunkX() * 16 + localX;
	int	worldZ = chunk.getChunkZ() * 16 + localZ;

	for (int y = 0; y < maxY; y++)
	{
		if (chunk.get(localX, y, localZ) == VoxelType::Air)
			continue;

		float	noise = _noise.noise3D(worldX * params.frequency, y * params.frequency, worldZ * params.frequency);

		float attenuation = 1.0f - glm::smoothstep((float)maxY * params.surfaceRatio, (float)maxY, (float)y);
		float value = noise * attenuation;

		if (value > params.threshold)
			chunk.set(localX, y, localZ, VoxelType::Air);
	}
}
