#ifndef GRAPHICS_H
#define GRAPHICS_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "textures.h"
#include <vector>
#include <memory>
#include <array>

extern int WIDTH;
extern int HEIGHT;

// player stuff
extern const float player_height;
extern bool jumped;
extern bool noclip;

// render distancer (in OpenGL terms)
extern const int render_dist;

// texture array
extern std::array<std::unique_ptr<Texture>, TEX_COUNT> Textures;

extern void RegisterTextures();

// a function to draw a cube
extern void draw_cube(Texture* Current_tex);

// Initializes the graphics stuff
extern void init_graphics();
extern void init_GL(int WIDTH, int HEIGHT);

// Updates stuff, handles user input
void update(GLFWwindow* window);

// Renders stuff to the screen
void render();

#endif
