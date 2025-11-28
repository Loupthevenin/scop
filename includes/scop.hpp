#pragma once

#define GLFW_INCLUDE_NONE

// Glad : charge dynamiquement OpenGL
#include <glad/glad.h>

// GLFW : création fenêtre & contexte
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#define WIDTH 800
#define HEIGHT 600
