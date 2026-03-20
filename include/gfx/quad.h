#ifndef QUAD_H
#define QUAD_H

#include "glew.h"
#include "gfx/text.h"
#include "gfx/textures.h"
// This file handles text rendering and other things

// vertices for the quad
extern GLfloat quad_vertices[];

extern GLfloat quad_texcoords[];

// VBOs
extern GLuint quadVBO;
extern GLuint quadUV;

// Initialize the VBOs
extern void init_quadvbo();

extern void init_quaduv();

extern void Draw_Quad(Texture* tex);

extern void Draw_Quad_Char(Texture* tex, float x, float y, float z, float u0, float u1, float v0, float v1, float size);

#endif
