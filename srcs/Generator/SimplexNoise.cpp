#include "SimplexNoise.hpp"

static const float	F2 = 0.5f * (sqrtf(3.0f) - 1.0f);
static const float	G2 = (3.0f - sqrtf(3.0f)) / 6.0f;

static void	fisherYates(uint64_t seed, uint8_t (&toMix)[256])
{
	for (int i = 255; i > 0; i--) {
		seed = seed * 6364136223846793005ull + 1442695040888963407ull;
		int		j = seed % (i + 1);

		uint8_t	tmp = toMix[i];
		toMix[i]	= toMix[j];
		toMix[j]	= tmp;
	}
}

static float	dot(const int* grad, float x, float y) {
	return grad[0] * x + grad[1] * y;
}

static void	skew(float x, float y, int& i, int& j)
{
	float s = (x + y) * F2;
	i = (int)floorf(x + s);
	j = (int)floorf(y + s);
}

static void	computeCorners(float x, float y, int i, int j,
							float& x0, float& y0,
							float& x1, float& y1,
							float& x2, float& y2,
							int& i1,   int& j1)
{
	float	t = (float)(i + j) * G2;
	x0 = x - (float)i + t;
	y0 = y - (float)j + t;

	if (x0 > y0)
	{
		i1 = 1;
		j1 = 0;
	}
	else
	{
		i1 = 0;
		j1 = 1;
	}

	x1 = x0 - (float)i1 + G2;
	y1 = y0 - (float)j1 + G2;
	x2 = x0 - 1.0f + 2.0f * G2;
	y2 = y0 - 1.0f + 2.0f * G2;
}


SimplexNoise::SimplexNoise(uint64_t seed) {
	uint8_t base[256];
	for (int i = 0; i < 256; i++)
		base[i] = (uint8_t)i;

	fisherYates(seed, base);

	for (int i = 0; i < 512; i++)
		_perm[i] = base[i & 255];
}

SimplexNoise::~SimplexNoise(void)
{	
}

float	SimplexNoise::_cornerContribution(float x, float y, int i, int j) const
{
	static const int GRADIENTS[8][2] = {
		{ 1, 1}, {-1, 1}, { 1,-1}, {-1,-1},
		{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}
	};

	float	t = 0.5f - x * x - y * y;
	if (t < 0.0f)
		return 0.0f;

	int gi = _perm[(i + _perm[j & 255]) & 255] % 8;
	t *= t;
	return t * t * dot(GRADIENTS[gi], x, y);
}

float	SimplexNoise::noise2D(float x, float y) const
{
	int i;
	int	j;
	skew(x, y, i, j);

	float x0, y0, x1, y1, x2, y2;
	int   i1, j1;
	computeCorners(x, y, i, j, x0, y0, x1, y1, x2, y2, i1, j1);

	float n0 = _cornerContribution(x0, y0, i,      j);
	float n1 = _cornerContribution(x1, y1, i + i1, j + j1);
	float n2 = _cornerContribution(x2, y2, i + 1,  j + 1);

	return 70.0f * (n0 + n1 + n2);
}

float	SimplexNoise::fractal2D(float x, float y, int octaves, float frequency, float amplitude, float lacunarity, float persistence) const
{
	float result   = 0.0f;
	float maxValue = 0.0f;

	for (int i = 0; i < octaves; i++)
	{
		result   += noise2D(x * frequency, y * frequency) * amplitude;
		maxValue += amplitude;

		frequency   *= lacunarity;
		amplitude   *= persistence;
	}

	return result / maxValue;
}
