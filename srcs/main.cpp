#include "opengl.hpp"
#include "utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: Wrong number of argument." << std::endl;
		std::cout << "Usage: ./ft_vox <SEED>" << std::endl;
		return 1;
	}

	std::cout << hashSeed(argv[1]) << std::endl;

	return 0;
}
