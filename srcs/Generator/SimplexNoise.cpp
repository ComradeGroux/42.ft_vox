#include "SimplexNoise.hpp"

#include "glm.hpp"

static const float	F2 = 0.5f * (sqrtf(3.0f) - 1.0f);
static const float	G2 = (3.0f - sqrtf(3.0f)) / 6.0f;

static const float	F3 = 1.0f / 3.0f;
static const float	G3 = 1.0f / 6.0f;

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

static void	skew2D(float x, float y, int& i, int& j)
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

float	SimplexNoise::_cornerContribution2D(float x, float y, int i, int j) const
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
	skew2D(x, y, i, j);

	float x0, y0, x1, y1, x2, y2;
	int   i1, j1;
	computeCorners(x, y, i, j, x0, y0, x1, y1, x2, y2, i1, j1);

	float n0 = _cornerContribution2D(x0, y0, i,      j);
	float n1 = _cornerContribution2D(x1, y1, i + i1, j + j1);
	float n2 = _cornerContribution2D(x2, y2, i + 1,  j + 1);

	return 70.0f * (n0 + n1 + n2);
}


/**
 * octaves:		number of layer of noise generated
 * frequency:	zoom in/out
 * amplitude:	control intensity of the layer
 * lacunarity:	how much the frequency augment between layer
 * persistence:	how much the amplitude augment between layer
 */
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

static void	skew3D(float x, float y, float z,
					int& i, int& j, int& k)
{
	float	s = (x + y + z) * F3;

	i = (int)floorf(x + s);
	j = (int)floorf(y + s);
	k = (int)floorf(z + s);
}

static void	computeOrigin3D(float x, float y, float z,
							int i, int j, int k,
							float& x0, float& y0, float& z0)
{
	float t = (float)(i + j + k) * G3;
	x0 = x - (float)i + t;
	y0 = y - (float)j + t;
	z0 = z - (float)k + t;
}

static void	computeOffsets3D(float x0, float y0, float z0,
							int& i1, int& j1, int& k1,
							int& i2, int& j2, int& k2)
{
	if (x0 >= y0)
	{
		if (y0 >= z0)
		{
			i1=1;
			j1=0;
			k1=0;
			i2=1;
			j2=1;
			k2=0;
		}
		else if (x0 >= z0)
		{
			i1=1;
			j1=0;
			k1=0;
			i2=1;
			j2=0;
			k2=1;
		}
		else
		{
			i1=0;
			j1=0;
			k1=1;
			i2=1;
			j2=0;
			k2=1;
		}
	}
	else
	{
		if (y0 < z0)
		{
			i1=0;
			j1=0;
			k1=1;
			i2=0;
			j2=1;
			k2=1;
		}
		else if (x0 < z0)
		{
			i1=0;
			j1=1;
			k1=0;
			i2=0;
			j2=1;
			k2=1;
		}
		else
		{
			i1=0;
			j1=1;
			k1=0;
			i2=1;
			j2=1;
			k2=0;
		}
	}
}

static void	computeCorners3D(float x0, float y0, float z0,
							int i1, int j1, int k1,
							int i2, int j2, int k2,
							float& x1, float& y1, float& z1,
							float& x2, float& y2, float& z2,
							float& x3, float& y3, float& z3)
{
	x1 = x0 - (float)i1 + G3;
	y1 = y0 - (float)j1 + G3;
	z1 = z0 - (float)k1 + G3;

	x2 = x0 - (float)i2 + 2.0f * G3;
	y2 = y0 - (float)j2 + 2.0f * G3;
	z2 = z0 - (float)k2 + 2.0f * G3;

	x3 = x0 - 1.0f + 3.0f * G3;
	y3 = y0 - 1.0f + 3.0f * G3;
	z3 = z0 - 1.0f + 3.0f * G3;
}

float	SimplexNoise::_cornerContribution3D(float x, float y, float z, int i, int j, int k) const
{
	static const int GRADIENTS3D[12][3] = {
		{ 1, 1, 0}, {-1, 1, 0}, { 1,-1, 0}, {-1,-1, 0},
		{ 1, 0, 1}, {-1, 0, 1}, { 1, 0,-1}, {-1, 0,-1},
		{ 0, 1, 1}, { 0,-1, 1}, { 0, 1,-1}, { 0,-1,-1}
	};

	float	t = 0.6f - x*x - y*y - z*z;
	if (t < 0.0f)
		return 0.0f;

	int	gi = _perm[(i + _perm[(j + _perm[k & 255]) & 255]) & 255] % 12;
	t *= t;

	return t * t * (GRADIENTS3D[gi][0]*x + GRADIENTS3D[gi][1]*y + GRADIENTS3D[gi][2]*z);
}

float	SimplexNoise::noise3D(float x, float y, float z) const
{
	int i, j, k;
	skew3D(x, y, z, i, j, k);

	float x0, y0, z0;
	computeOrigin3D(x, y, z, i, j, k, x0, y0, z0);

	int i1, j1, k1, i2, j2, k2;
	computeOffsets3D(x0, y0, z0, i1, j1, k1, i2, j2, k2);

	float x1, y1, z1, x2, y2, z2, x3, y3, z3;
	computeCorners3D(x0, y0, z0, i1, j1, k1, i2, j2, k2,
					x1, y1, z1, x2, y2, z2, x3, y3, z3);

	float	n0 = _cornerContribution3D(x0, y0, z0, i,		 j,		 k);
	float	n1 = _cornerContribution3D(x1, y1, z1, i + i1, j + j1, k + k1);
	float	n2 = _cornerContribution3D(x2, y2, z2, i + i2, j + j2, k + k2);
	float	n3 = _cornerContribution3D(x3, y3, z3, i + 1,  j + 1,  k + 1);

	return 32.0f * (n0 + n1 + n2 + n3);
}
