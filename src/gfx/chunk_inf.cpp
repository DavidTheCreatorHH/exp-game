#include "world/chunks.h"
#include "gfx/graphics.h"
#include "world/map.h"
#include "gfx/textures.h"
#include "gfx/cinf.h"
#include "config.h"
#include <iostream>
#include <cmath>

// load chunks around the player
void load_chunks(double playerX, double playerZ){
	// get the position of the player in chunk terms
	int player_chunk_x = static_cast<int>(std::floor(playerX / CHUNK_WIDTH));
	int player_chunk_z = static_cast<int>(std::floor(playerZ / CHUNK_WIDTH));

	// go around all the chunks in the current area of the player
	for (int dx = -VIEW_DISTANCE_X; dx <= VIEW_DISTANCE_X; ++dx){
		for (int dz = -VIEW_DISTANCE_Z; dz <= VIEW_DISTANCE_Z; ++dz){
			// get current chunk
			int cx = player_chunk_x + dx;
			int cz = player_chunk_z + dz;

			long long key = chunkKey(cx, 0, cz);
			if (chunks.find(key) == chunks.end()) {
				create_chunk(cx, 0, cz); // create new chunk
				// update chunk VBOs
				update_chunks();
			}
		}
	}
}

// unload chunks around the player
void unload_chunks(double playerX, double playerZ){
	// get player position in chunk terms
    int playerChunkX = static_cast<int>(std::floor(playerX / CHUNK_WIDTH));
    int playerChunkZ = static_cast<int>(std::floor(playerZ / CHUNK_WIDTH));

	// go over every chunk
    for (auto it = chunks.begin(); it != chunks.end();){
    	// get chunk
        int dx = it->second.cx - playerChunkX;
        int dz = it->second.cz - playerChunkZ;

		// check if chunk is out of the view distance
		if (std::abs(dx) > VIEW_DISTANCE_X || std::abs(dz) > VIEW_DISTANCE_Z) {
			Chunk& chunk = it->second;

			// delete VBOs
			for (auto& [type, vbo] : chunk.vbos) {
				if (vbo != 0) glDeleteBuffers(1, &vbo);
			}
			chunk.vbos.clear();

			// delte UVs
			for (auto& [type, uvbo] : chunk.uvbos) {
				if (uvbo != 0) glDeleteBuffers(1, &uvbo);
			}
			chunk.uvbos.clear();

			chunk.vertexCounts.clear();

			// completely eradicate the chunk
			it = chunks.erase(it);
		}
		else {
			++it;	// otherwise just go to the next chunk
		}
    }
}
