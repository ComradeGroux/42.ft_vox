#pragma once

#include "WorldConstants.hpp"
#include "SimplexNoise.hpp"
#include "Chunk.hpp"

#include <cstdint>

class WorldGenerator {
	private:
		uint64_t		_seed;
		SimplexNoise	_noise;

		void	_generateColumn(Chunk& chunk, int localX, int localZ, const NoiseParams& params) const;

	public:
		WorldGenerator(uint64_t seed);
		~WorldGenerator(void);

		void	generate(Chunk& chunk) const;
};
