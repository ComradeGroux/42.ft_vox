#pragma once

#include "Section.hpp"

class Chunk {
	private:
		int						_chunkX;
		int						_chunkY;
		std::array<Section, 16>	_sections;

	public:
		Chunk(int chunkX, int chuckY);
		~Chunk(void);

		VoxelType	get(int x, int y, int z) const;
		void		set(int x, int y, int z, VoxelType type);

		const Section&	getSection(int sectionY) const;
};
