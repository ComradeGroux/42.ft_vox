#pragma once

#include "Chunk.hpp"
#include "ChunkMesh.hpp"
#include "WorldGenerator.hpp"
#include <unordered_map>
#include <memory>
#include <cmath>

class World {
	public:
		using ChunkMap = std::unordered_map<uint64_t, std::unique_ptr<Chunk>>;
		using MeshMap = std::unordered_map<uint64_t, std::unique_ptr<ChunkMesh>>;

	private:
		ChunkMap		_chunks;
		MeshMap			_meshes;

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
		const ChunkMap&	getChunks(void) const;

		const ChunkMesh*	getMesh(uint64_t key) const;
		const MeshMap&		getMeshes(void) const;

		Chunk&	loadChunk(int chunkX, int chunkZ);
		void	unloadChunk(int chunkX, int chunkZ);
		bool	isChunkLoaded(int chunkX, int chunkZ) const;
		void	updateLoadedChunks(float playerX, float playerZ);
};
