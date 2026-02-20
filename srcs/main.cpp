
#include "opengl.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <iostream>
#include "error.hpp"

unsigned int	validateArgument(int argc, char** argv)
{
	if (argc != 2)
		return WRONG_NUMBER_ARGUMENT_ID;

	if (strlen(argv[1]) < SEED_MIN_CHAR)
		return SEED_TO_SMALL_ID;
	else if (strlen(argv[1]) > SEED_MAX_CHAR)
		return SEED_TO_BIG_ID;



	return OK;
}

unsigned int	printErrorArgument(unsigned int err)
{
	switch (err)
	{
		case OK:
			return OK;

		case WRONG_NUMBER_ARGUMENT_ID:
			std::cerr << ERROR << WRONG_NUMBER_ARGUMENT_STR << std::endl;
			break;

		case SEED_TO_SMALL_ID:
			std::cerr << ERROR << SEED_TO_SMALL_STR << std::endl;
			break;
		case SEED_TO_BIG_ID:
			std::cerr << ERROR << SEED_TO_BIG_STR << std::endl;
			break;


	}



	std::cout << USAGE << std::endl;
	return err;
}

int main(int argc, char** argv)
{
	int err = printErrorArgument(validateArgument(argc, argv));
	if (err != OK)
		return err;



	return 0;
}
