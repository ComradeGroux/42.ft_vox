#pragma once

#include "WorldConstants.hpp"
#include "SimplexNoise.hpp"
#include "Chunk.hpp"

#include <cstdint>

class WorldGenerator {
	public:
		using WeightGrid = std::array<std::array<std::array<float, NUMBER_BIOME>, 16>, 16>;
		using RawWeightGrid = std::array<std::array<std::array<float, NUMBER_BIOME>, GRID_SIZE>, GRID_SIZE>;

	private:
		uint64_t		_seed;

		SimplexNoise	_terrainNoise;
		SimplexNoise	_temperatureNoise;
		SimplexNoise	_humidityNoise;

		int		_generateColumn(Chunk& chunk, int localX, int localZ, const std::array<float, NUMBER_BIOME>& weights, const TerrainParams& params) const;
		void	_generateCaves(Chunk& chunk, int localX, int maxY, int localZ, const CaveParams& params) const;

		std::array<float, NUMBER_BIOME>	_getBiomeWeights(float worldX, float worldZ) const;
		WeightGrid						_computeSmoothedWeights(const Chunk& chunk) const;

	public:
		WorldGenerator(uint64_t seed);
		~WorldGenerator(void);

		void	generate(Chunk& chunk) const;
};
