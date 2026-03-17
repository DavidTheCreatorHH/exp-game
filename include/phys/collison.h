#ifndef COLLISON_H
#define COLLISON_H

#include "gfx/graphics.h"
#include <cmath>

// Collison
// Axis-Applied Bounding Box for collison
struct AABB{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
};

// Get the player AABB
extern AABB getPlayerBox(float gCameraX, float gCameraY, float gCameraZ, float P_height);

// Get the cube AABB
extern AABB getCubeBox(float x, float y, float z);

// Check two points (AKA AABBs) are colliding
extern bool checkCollison(const AABB& a, const AABB& b);

// Check for collison between all the cubes
extern bool isColliding(float cx, float cy, float cz, float pheight);

#endif
