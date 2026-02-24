#pragma once

#include <cstdint>
#include <cmath>

class SimplexNoise {
	private:
		uint8_t	_perm[512];
	
		float	_cornerContribution2D(float x, float y, int i, int j) const;

		float	_cornerContribution3D(float x, float y, float z, int i, int j, int k) const;

	public:
		SimplexNoise(uint64_t seed);
		~SimplexNoise(void);

		float	noise2D(float x, float y) const;
		float	fractal2D(float x, float y, int octaves, float frequency, float amplitude, float lacunarity, float persistence) const;

		float	noise3D(float x, float y, float z) const;
};
