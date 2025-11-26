#pragma once

// GLFW inclut automatiquement le bon header OpenGL (gl3.h sur macOS)
#define GLFW_INCLUDE_NONE
#include "../lib/glfw/include/GLFW/glfw3.h"

#include <cstdlib>
#include <iostream>

// macOS : éviter warnings sur OpenGL déprécié
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
// OpenGL Core Profile sur macOS
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

// Taille de la fenêtre
#define WIDTH 800
#define HEIGHT 600
