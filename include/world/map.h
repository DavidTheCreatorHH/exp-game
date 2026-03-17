#ifndef MAP_H
#define MAP_H

#include "world/chunks.h"
#include "gfx/textures.h"
#include "gfx/cube.h"

extern void add_cube(Chunk& chunk, int x, int y, int z, btype type);

extern btype* get_cube(Chunk& chunk, int x, int y, int z);

extern btype get_cube_or_neighbor(const Chunk& chunk, int x, int y, int z);

// Create a cube with special attributes
extern void addCubewAttribs(float x, float y, float z, TextureID tex = TEX_TAL_TILE,
					bool canCollide = true, bool visible = true, float light = 1.0f,
					unsigned char r = 255, unsigned char g = 255, unsigned char b = 255);

// Creates the default chunks
extern void create_chunk(int ccx, int ccy, int ccz);
extern void update_chunks();

// this massive-ass function creates the world's cubes
extern int init_world(int seed = 0);

#endif
