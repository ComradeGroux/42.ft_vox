#pragma once

#include "Section.hpp"

class Chunk {
	private:
		int						_chunkX;
		int						_chunkZ;
		std::array<Section, 16>	_sections;

	public:
		Chunk(int chunkX, int chunkY);
		~Chunk(void);

		VoxelType	get(int x, int y, int z) const;
		void		set(int x, int y, int z, VoxelType type);

		int			getChunkX(void) const;
		int			getChunkZ(void) const;

		const Section&	getSection(int sectionY) const;
};
