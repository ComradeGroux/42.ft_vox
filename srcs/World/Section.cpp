#include "Section.hpp"

Section::Section(void) : _data{}, _solidCount(0)
{
}

Section::~Section(void)
{
}

VoxelType	Section::get(int x, int y, int z) const
{
	return _data[x + z * 16 + y * 256];
}

void	Section::set(int x, int y, int z, VoxelType type)
{
	VoxelType previous = _data[x + z * 16 + y * 256];

	if (previous == type)
		return;

	if (previous == VoxelType::Air && type != VoxelType::Air)
		_solidCount++;
	else if (previous != VoxelType::Air && type == VoxelType::Air)
		_solidCount--;

	_data[x + z * 16 + y * 256] = type;
}

bool	Section::isEmpty(void) const
{
	return (_solidCount == 0);
}
