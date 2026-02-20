#include "Chunk.hpp"

Chunk::Chunk(int chunkX, int chunkY) : _chunkX(chunkX), _chunkY(chunkY)
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

const Section&	Chunk::getSection(int sectionY) const
{
	return _sections[sectionY];
}
