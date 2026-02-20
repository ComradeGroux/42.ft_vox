#pragma once

#include "glad.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define cgl(x) GLClearError(); x; GLGetError(#x, __FILE__, __LINE__)

void	GLClearError();
void	GLGetError(const char *function, const char *file, int line);
