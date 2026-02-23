#pragma once

#include <cstdint>
#include <cmath>

class SimplexNoise {
	private:
		uint8_t	_perm[512];
	
		float	_cornerContribution(float x, float y, int i, int j) const;

	public:
		SimplexNoise(uint64_t seed);
		~SimplexNoise(void);

		float	noise2D(float x, float y) const;
		float	fractal2D(float x, float y, int octaves, float frequency, float amplitude, float lacunarity, float persistence) const;
};
