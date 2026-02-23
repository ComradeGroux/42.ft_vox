#pragma once

#include "glad.h"

#include <iostream>

void	GLClearError();
void	GLGetError(const char* function, const char* file, int line);

#ifdef DEBUG
	#define cgl(x) GLClearError(); x; GLGetError(#x, __FILE__, __LINE__)
#else
	#define cgl(x) x
#endif
