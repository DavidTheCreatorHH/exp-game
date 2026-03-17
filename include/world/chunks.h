#ifndef CHUNKS_H
#define CHUNKS_H

#include "gfx/cube.h"
#include "phys/collison.h"
#include "config.h"
#include <vector>
#include <unordered_map>
#include <GL/glew.h>

enum btype : uint8_t {
	_AIR,
	_GROUND,
	_LEGACY_GROUND,
	_DIRT,
	_LAVA,
	_COLD_LAVA,
	_OBSIDIAN,
	_TASR,
	_EREL_CRYSTAL,
	_TAL_TILE,
	_TEST,
	_COUNT
};

// I hate dynamic vectors
struct Chunk {
	int cx, cy, cz;
	std::vector<btype> cubes;
	AABB bounds;

	// Rendering
	std::unordered_map<btype, GLuint> vbos;
	std::unordered_map<btype, GLuint> uvbos;
	std::unordered_map<btype, int> vertexCounts;

	bool visible = true;
	
	bool dirty = false;
	
	Chunk() : cx(0), cy(0), cz(0), cubes(CHUNK_WIDTH*CHUNK_HEIGHT*CHUNK_WIDTH, _AIR) {}
    Chunk(int x, int y, int z) : cx(x), cy(y), cz(z),
        cubes(CHUNK_WIDTH*CHUNK_HEIGHT*CHUNK_WIDTH, _AIR) {}
	
	inline int index(int x, int y, int z) const {
        return x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z);
    }

    btype get(int x, int y, int z) const {
        return cubes[index(x,y,z)];
    }

    void set(int x, int y, int z, btype type) {
        cubes[index(x,y,z)] = type;
    }
	
};

extern std::unordered_map<long long, Chunk> chunks;

// Chunk get
long long chunkKey(int x, int y, int z);
extern Chunk* get_chunk_ptr(int cx, int cy, int cz);
extern Chunk& get_chunk(int cx, int cy, int cz);

// Generate a chunk's VBO after adding cubes
void buildChunkVBO(Chunk& chunk);

#endif
