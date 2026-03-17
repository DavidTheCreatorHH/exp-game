#ifndef CUBE_H
#define CUBE_H

#include <stdbool.h>
#include "gfx/textures.h"

// VBO and UV for the cube object
extern GLuint cubeVBO;
extern GLuint cubeUV;

struct Cube {
    float x, y, z;
    TextureID tex;

    bool canCollide;
    bool visible;
    unsigned char r, g, b;

    TextureID type;
    float light;

    Cube(float _x, float _y, float _z, TextureID _tex = TEX_TAL_TILE,
         bool _canCollide = true, bool _visible = true, float _light = 1.0f,
         unsigned char _r = 255, unsigned char _g = 255, unsigned char _b = 255)
        : x(_x), y(_y), z(_z), tex(_tex),
          canCollide(_canCollide), visible(_visible),
          r(_r), g(_g), b(_b),
          type(_tex), light(_light) {}
};

// vertices for the cube
extern GLfloat cube_vertices[];

// Coordinates for the texture
extern GLfloat cube_texcoords[];

extern void init_cubevbo();

extern void init_cube_uvbo();

#endif
