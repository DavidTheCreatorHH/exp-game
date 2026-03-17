#ifndef CONTROLS_H
#define CONTROLS_H
#include <cmath>
#include "gfx/cam.h"

extern void destroyBlock(camera& cam);

extern void placeBlock(camera& cam, btype blockType);

extern void set_block(int x, int y, int z, btype type);

#endif
