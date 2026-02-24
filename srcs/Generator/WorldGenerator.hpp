#pragma once

#include "WorldConstants.hpp"
#include "SimplexNoise.hpp"
#include "Chunk.hpp"

#include <cstdint>

class WorldGenerator {
	private:
		uint64_t		_seed;
		SimplexNoise	_noise;

		int		_generateColumn(Chunk& chunk, int localX, int localZ, const TerrainParams& params) const;
		void	_generateCaves(Chunk& chunk, int localX, int maxY, int localZ, const CaveParams& params) const;

	public:
		WorldGenerator(uint64_t seed);
		~WorldGenerator(void);

		void	generate(Chunk& chunk) const;
};
