#include "world/chunks.h"
#include "gfx/cube.h"
#include "phys/collison.h"
#include "config.h"
#include <algorithm>
#include <iostream>

std::unordered_map<long long, Chunk> chunks;

long long chunkKey(int x, int y, int z) {
    return ((long long)(x & 0x1FFFFF) << 42) |
           ((long long)(y & 0x1FFFFF) << 21) |
           (z & 0x1FFFFF);
}

Chunk* get_chunk_ptr(int cx, int cy, int cz){
    long long key = chunkKey(cx, cy, cz);

    auto it = chunks.find(key);
    if(it == chunks.end())
        return nullptr;

    return &it->second;
}

Chunk& get_chunk(int cx, int cy, int cz){
    long long key = chunkKey(cx, cy, cz);
    auto& chunk = chunks[key];

    chunk.cx = cx;
    chunk.cy = cy;
    chunk.cz = cz;

    return chunk;
}
