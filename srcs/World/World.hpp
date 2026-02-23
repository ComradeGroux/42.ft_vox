#pragma once

#include "Chunk.hpp"
#include "WorldGenerator.hpp"
#include <unordered_map>
#include <memory>
#include <cmath>

class World {
	private:
		using ChunkMap = std::unordered_map<uint64_t, std::unique_ptr<Chunk>>;
		ChunkMap		_chunks;

		WorldGenerator	_generator;

		int	_lastPlayerChunkX;
		int	_lastPlayerChunkZ;

		static	uint64_t	_chunkKey(int chunkX, int chunkZ);

	public:
		World(uint64_t seed);
		~World(void);

		VoxelType	get(int x, int y, int z) const;
		void		set(int x, int y, int z, VoxelType type);

		Chunk*			getChunk(int chunkX, int chunkZ);
		const Chunk*	getChunk(int chunkX, int chunkZ) const;

		Chunk&	loadChunk(int chunkX, int chunkZ);
		void	unloadChunk(int chunkX, int chunkZ);
		bool	isChunkLoaded(int chunkX, int chunkZ) const;
		void	updateLoadedChunks(float playerX, float playerZ);
};
