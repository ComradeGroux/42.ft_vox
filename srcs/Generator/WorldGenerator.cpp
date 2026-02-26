#include "WorldGenerator.hpp"

#include "glm.hpp"

WorldGenerator::WorldGenerator(uint64_t seed) : _seed(seed), _terrainNoise(seed), _temperatureNoise(seed + 1), _humidityNoise(seed + 2)
{
}

WorldGenerator::~WorldGenerator(void)
{
}

void	WorldGenerator::generate(Chunk& chunk) const
{
	WeightGrid	weights = _computeSmoothedWeights(chunk);
	int			dominantBiome = 0;

	int			height = 0;
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			dominantBiome = 0;
			for (int i = 1; i < NUMBER_BIOME; i++)
			{
				if (weights[x][z][i] > weights[x][z][dominantBiome])
					dominantBiome = i;
			}

			height = _generateColumn(chunk, x, z, weights[x][z], dominantBiome);
			_generateCaves(chunk, x, height, z, BIOMES[dominantBiome].cave);
		}
	}
}

int	WorldGenerator::_generateColumn(Chunk& chunk, int localX, int localZ, const std::array<float, NUMBER_BIOME>& weights, int dominantBiome) const
{
	int	worldX = chunk.getChunkX() * 16 + localX;
	int	worldZ = chunk.getChunkZ() * 16 + localZ;

	float	nonOceanHeight = 0.0f;
	float	nonOceanWeight = 0.0f;
	float	totalOceanWeight = 0.0f;
	for (int i = 0; i < NUMBER_BIOME; i++)
	{
		if (BIOMES[i].isOcean)
		{
			totalOceanWeight += weights[i];
			continue;
		}

		const TerrainParams& terrain = BIOMES[i].terrain;
		float noise = _terrainNoise.fractal2D((float)worldX, (float)worldZ, terrain);
		float height = terrain.terrain_min_height + (noise + 1.0f) / 2.0f * (terrain.terrain_max_height - terrain.terrain_min_height);
		nonOceanHeight += height * weights[i];
		nonOceanWeight += weights[i];
	}

	int height = (int)glm::min(nonOceanHeight, glm::mix(nonOceanHeight, (float)SEA_LEVEL, totalOceanWeight));

	VoxelType	type;
	VoxelType	surfaceType = BIOMES[dominantBiome].terrain.ground_type;
	VoxelType	subSurfaceType = BIOMES[dominantBiome].terrain.ground_depth_type;

	if (height < SEA_LEVEL)
	{
		surfaceType = VoxelType::Sand;
		subSurfaceType = VoxelType::Sand;
	}
	for (int y = 0; y <= height; y++)
	{
		if (y == height)
			type = surfaceType;
		else if (y >= height - BIOMES[dominantBiome].terrain.ground_depth)
			type = subSurfaceType;
		else
			type = VoxelType::Stone;

		chunk.set(localX, y, localZ, type);
	}
	if (height < SEA_LEVEL)
	{
		for (int y = height + 1; y <= SEA_LEVEL; y++)
			chunk.set(localX, y, localZ, VoxelType::Water);
	}
	return height;
}

void	WorldGenerator::_generateCaves(Chunk& chunk, int localX, int maxY, int localZ, const CaveParams& params) const
{
	int worldX = chunk.getChunkX() * 16 + localX;
	int	worldZ = chunk.getChunkZ() * 16 + localZ;

	float	minAttenuation = 0.2f;

	for (int y = 1; y < maxY; y++)
	{
		if (chunk.get(localX, y, localZ) == VoxelType::Air)
			continue;

		float	caveNoise = _terrainNoise.fractal3D(worldX * params.frequency, y * params.frequencyY, worldZ * params.frequency, params);
		float	densityNoise = _terrainNoise.noise3D(worldX * 0.005f, y * 0.005f, worldZ * 0.005f);

		float	attenuation = minAttenuation + (1.0f - minAttenuation) * (1.0f - glm::smoothstep((float)maxY * params.surfaceRatio, (float)maxY, (float)y));

		float	densityFactor = glm::smoothstep(-0.5f, 0.5f, densityNoise);
		float	modulatedThreshold = params.threshold - (densityFactor * 0.2f);

		if (caveNoise * attenuation > modulatedThreshold)
			chunk.set(localX, y, localZ, VoxelType::Air);
	}
}

std::array<float, NUMBER_BIOME>	WorldGenerator::_getBiomeWeights(float worldX, float worldZ) const
{
	float	temperature = (_temperatureNoise.noise2D(worldX * 0.0008f, worldZ * 0.0008f) + 1.0f) / 2.0f;
	float	humidity = (_humidityNoise.noise2D(worldX * 0.0008f, worldZ * 0.0008f) + 1.0f) / 2.0f;

	std::array<float, NUMBER_BIOME>	weights;
	float							totalWeight = 0.0f;

	for (int i = 0; i < NUMBER_BIOME; i++)
	{
		float	dt = temperature - BIOMES[i].temperature;
		float	dh = humidity - BIOMES[i].humidity;
		float	distance = sqrtf(dt * dt + dh * dh);

		weights[i] = 1.0f / (distance * distance + 0.0001f);
		weights[i] = weights[i] * weights[i] * weights[i] * weights[i];
		totalWeight += weights[i];
	}

	for (int i = 0; i < NUMBER_BIOME; i++)
		weights[i] /= totalWeight;

	return weights;
}

WorldGenerator::WeightGrid	WorldGenerator::_computeSmoothedWeights(const Chunk& chunk) const
{
	RawWeightGrid	rawWeights;
	for (int gx = 0; gx < GRID_SIZE; gx++)
	{
		for (int gz = 0; gz < GRID_SIZE; gz++)
		{
			float worldX = (chunk.getChunkX() * 16 - BLEND_RADIUS) + gx;
			float worldZ = (chunk.getChunkZ() * 16 - BLEND_RADIUS) + gz;
			rawWeights[gx][gz] = _getBiomeWeights(worldX, worldZ);
		}
	}

	constexpr float kernelSize = (2 * BLEND_RADIUS + 1) * (2 * BLEND_RADIUS + 1);
	WeightGrid smoothedWeights = {};

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			for (int dx = -BLEND_RADIUS; dx <= BLEND_RADIUS; dx++)
			{
				for (int dz = -BLEND_RADIUS; dz <= BLEND_RADIUS; dz++)
				{
					int gx = x + BLEND_RADIUS + dx;
					int gz = z + BLEND_RADIUS + dz;

					for (int i = 0; i < NUMBER_BIOME; i++)
						smoothedWeights[x][z][i] += rawWeights[gx][gz][i];
				}
			}

			for (int i = 0; i < NUMBER_BIOME; i++)
				smoothedWeights[x][z][i] /= kernelSize;
		}
	}

	return smoothedWeights;
}
