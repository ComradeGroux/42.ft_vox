#pragma once

#include <cstdint>
#include <array>

enum class VoxelType : uint8_t {
	Air 	= 0,
	Grass	= 1,
	Dirt	= 2,
	Stone	= 3,
	Sand	= 4,
	Water	= 5
};

/**
 * A section is a part of a chunk.
 * 
 * While each chunk is 16x256x16 bloc wide, a section is only 16x16x16.
 * It means that a chunk is composed of 8 section
 * 
 */
class Section {
	private:
		std::array<VoxelType, 4096>	_data;
		uint8_t						_solidCount;
	
	public:
		Section(void);
		~Section(void);

		VoxelType	get(int x, int y, int z) const;
		void		set(int x, int y, int z, VoxelType type);

		bool		isEmpty(void) const;
};
