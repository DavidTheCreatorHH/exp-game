#ifndef INIT_H
#define INIT_H

#define GLFW_INCLUDE_NONE
#include "glfw3.h"
#include "glew.h"
#include "world/chunks.h"

// Initialize OpenGL
extern void init_GL(int WIDTH, int HEIGHT, float renderDist);

// Init graphics
extern void init_graphics();

// Init ImGui
extern void init_imgui(GLFWwindow* win);

extern void init_cam();

// Init Camera Y
extern void init_y_cam(float& cam_y);

#endif
