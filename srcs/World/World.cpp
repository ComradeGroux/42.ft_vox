#include "World.hpp"

World::World(uint64_t seed) : _generator(seed), _lastPlayerChunkX(__INT_MAX__), _lastPlayerChunkZ(__INT_MAX__)
{
}

World::~World(void)
{
}

VoxelType	World::get(int x, int y, int z) const
{
	int	chunkX = x >> 4;
	int	chunkZ = z >> 4;
	int	localX = x - chunkX * 16;
	int	localZ = z - chunkZ * 16;

	const Chunk*	chunk = getChunk(chunkX, chunkZ);
	if (!chunk)
		return VoxelType::Air;

	return chunk->get(localX, y, localZ);
}

void	World::set(int x, int y, int z, VoxelType type)
{
	int		chunkX = x >> 4;
	int		chunkZ = z >> 4;
	Chunk*	chunk = getChunk(chunkX, chunkZ);
	if (!chunk)
		return;

	chunk->set(x & 15, y, z & 15, type);
}

uint64_t	World::_chunkKey(int chunkX, int chunkZ)
{
	return ((uint64_t)chunkX << 32) | (uint32_t)chunkZ;
}

Chunk*	World::getChunk(int chunkX, int chunkZ)
{
	ChunkMap::iterator	it = _chunks.find(_chunkKey(chunkX, chunkZ));
	if (it == _chunks.end())
		return nullptr;

	return it->second.get();
}

const Chunk*	World::getChunk(int chunkX, int chunkZ) const
{
	ChunkMap::const_iterator	it = _chunks.find(_chunkKey(chunkX, chunkZ));
	if (it == _chunks.end())
		return nullptr;

	return it->second.get();
}

Chunk&	World::loadChunk(int chunkX, int chunkZ)
{
	uint64_t							key = _chunkKey(chunkX, chunkZ);
	std::pair<ChunkMap::iterator, bool> result = _chunks.emplace(key, std::make_unique<Chunk>(chunkX, chunkZ));

	if (result.second)
		_generator.generate(*result.first->second);

	return *result.first->second;
}

void	World::unloadChunk(int chunkX, int chunkZ)
{
	_chunks.erase(_chunkKey(chunkX, chunkZ));
}

bool	World::isChunkLoaded(int chunkX, int chunkZ) const
{
	return _chunks.count(_chunkKey(chunkX, chunkZ)) > 0;
}

void	World::updateLoadedChunks(float playerX, float playerZ)
{
	int	playerChunkX = (int)std::floor(playerX) >> 4;
	int	playerChunkZ = (int)std::floor(playerZ) >> 4;

	if (playerChunkX == _lastPlayerChunkX && playerChunkZ == _lastPlayerChunkZ)
		return;

	_lastPlayerChunkX = playerChunkX;
	_lastPlayerChunkZ = playerChunkZ;

	for (int dx = -RENDER_DISTANCE; dx < RENDER_DISTANCE; dx++)
	{
		for (int dz = -RENDER_DISTANCE; dz < RENDER_DISTANCE; dz++)
		{
			if (dx * dx + dz * dz <= RENDER_DISTANCE * RENDER_DISTANCE)
			{
				if (!isChunkLoaded(playerChunkX + dx, playerChunkZ + dz))
					loadChunk(playerChunkX + dx, playerChunkZ + dz);
			}
		}
	}

	for (ChunkMap::iterator it = _chunks.begin(); it != _chunks.end();)
	{
		int	dx = it->second->getChunkX() - playerChunkX;
		int	dz = it->second->getChunkZ() - playerChunkZ;

		if (dx * dx + dz * dz > RENDER_DISTANCE * RENDER_DISTANCE)
			it = _chunks.erase(it);
		else
			it++;
	}
}
