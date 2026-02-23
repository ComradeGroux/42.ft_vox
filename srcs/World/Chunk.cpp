#include "Chunk.hpp"

Chunk::Chunk(int chunkX, int chunkZ) : _chunkX(chunkX), _chunkZ(chunkZ)
{
}

Chunk::~Chunk(void)
{
}

VoxelType Chunk::get(int x, int y, int z) const {
	return _sections[y / 16].get(x, y % 16, z);
}

void Chunk::set(int x, int y, int z, VoxelType type) {
	_sections[y / 16].set(x, y % 16, z, type);
}

int	Chunk::getChunkX(void) const
{
	return _chunkX;
}

int Chunk::getChunkZ(void) const
{
	return _chunkZ;
}

const Section&	Chunk::getSection(int sectionY) const
{
	return _sections[sectionY];
}
