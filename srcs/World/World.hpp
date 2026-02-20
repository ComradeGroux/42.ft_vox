#pragma once

#include "Chunk.hpp"
#include <unordered_map>
#include <memory>
#include <cmath>

class World {
	private:
		using ChunkMap = std::unordered_map<uint64_t, std::unique_ptr<Chunk>>;
		ChunkMap	_chunks;

		static	uint64_t	_chunkKey(int chunkX, int chunkY);

	public:
		World(void);
		~World(void);

		VoxelType	get(int x, int y, int z) const;
		void		set(int x, int y, int z, VoxelType type);

		Chunk*			getChunk(int chunkX, int chunkY);
		const Chunk*	getChunk(int chunkX, int chunkY) const;

		Chunk&	loadChunk(int chunkX, int chunkZ);
		void	unloadChunk(int chunkX, int chunkZ);
		bool	isChunkLoaded(int chunkX, int chunkZ) const;
};
