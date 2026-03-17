#include "phys/collison.h"
#include "gfx/graphics.h"
#include "world/chunks.h"
#include "gfx/cube.h"
#include <cmath>

// define block size (why?)
#define BLOCK_SIZE 1.0f

// the AABB box for the player
AABB getPlayerBox(float px, float py, float pz, float height){
    float halfSize = 0.4f; // player width in X/Z
    return {
        px - halfSize, py - height, pz - halfSize,
        px + halfSize, py, pz + halfSize
    };
}

// the AABB box for Cubes
AABB getCubeBox(float x, float y, float z){
    return { x, y, z, x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE };
}

// check if two objects are colliding
bool checkCollision(const AABB& a, const AABB& b){
	return	(a.minX < b.maxX && a.maxX > b.minX) &&
			(a.minY < b.maxY && a.maxY > b.minY) &&
			(a.minZ < b.maxZ && a.maxZ > b.minZ);
}

// function for collision checking
bool isColliding(float px, float py, float pz, float playerHeight) {
    AABB playerBox = getPlayerBox(px, py, pz, playerHeight);

    // determine which chunk the player is in
    int pcx = static_cast<int>(std::floor(px / CHUNK_WIDTH));
    int pcy = static_cast<int>(std::floor(py / CHUNK_HEIGHT));
    int pcz = static_cast<int>(std::floor(pz / CHUNK_WIDTH));

	// check the chunk the player is in
	long long key = chunkKey(pcx, pcy, pcz);
	if (!chunks.count(key)) return false;	// chunk doesn't exist

	const Chunk& chunk = chunks[key];

	// Convert player bounding box to chunk-local coordinates
	const float EPS = 0.001f;

	int minX = std::max(0, static_cast<int>(std::floor(playerBox.minX - EPS)) - chunk.cx * CHUNK_WIDTH);
	int maxX = std::min(CHUNK_WIDTH - 1, static_cast<int>(std::floor(playerBox.maxX + EPS)) - chunk.cx * CHUNK_WIDTH);

	int minY = std::max(0, static_cast<int>(std::floor(playerBox.minY - EPS)) - chunk.cy * CHUNK_HEIGHT);
	int maxY = std::min(CHUNK_HEIGHT - 1, static_cast<int>(std::floor(playerBox.maxY + EPS)) - chunk.cy * CHUNK_HEIGHT);

	int minZ = std::max(0, static_cast<int>(std::floor(playerBox.minZ - EPS)) - chunk.cz * CHUNK_WIDTH);
	int maxZ = std::min(CHUNK_WIDTH - 1, static_cast<int>(std::floor(playerBox.maxZ + EPS)) - chunk.cz * CHUNK_WIDTH);

	// Iterate only over blocks the player could collide with
	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			for (int z = minZ; z <= maxZ; z++) {
			btype type = chunk.get(x, y, z);
			if (type == _AIR) continue; // skip empty blocks

			// Optional: if you have per-block collision flags
			// if (!canCollide(type)) continue;

			AABB cubeBox = getCubeBox(x + chunk.cx * CHUNK_WIDTH,
			       y + chunk.cy * CHUNK_HEIGHT,
			       z + chunk.cz * CHUNK_WIDTH);

			if (checkCollision(playerBox, cubeBox))
			return true;
			}
		}
	}
			
    return false;
}
