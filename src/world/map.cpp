#include "world/chunks.h"
#include "gfx/textures.h"
#include "funcs/rand.h"
#include "gfx/cube.h"
#include "out.h"
#include <unordered_map>
#include <cmath>
#include <vector>

// utils
btype get_cube(Chunk& chunk, int x, int y, int z){
    return chunk.cubes[chunk.index(x,y,z)];
}

void add_cube(Chunk& chunk, int x, int y, int z, btype type){
    chunk.set(x,y,z,type);
}

void push_cube_faces(std::vector<float>& vertices, std::vector<float>& uvs,
                     const Chunk& chunk, int x, int y, int z, bool faces[6]) {
    for (int f = 0; f < 6; f++) {
        if (!faces[f]) continue;

        // Each face now has 2 triangles, i.e., 6 vertices
        for (int i = 0; i < 6; i++) {
            int idx = f * 2 * 3 * 3 + i * 3; // face offset * 2 triangles * 3 vertices * 3 coords
            vertices.push_back(cube_vertices[idx + 0] + x + chunk.cx * CHUNK_WIDTH);
            vertices.push_back(cube_vertices[idx + 1] + y + chunk.cy * CHUNK_HEIGHT);
            vertices.push_back(cube_vertices[idx + 2] + z + chunk.cz * CHUNK_WIDTH);

            int uv_idx = f * 2 * 3 * 2 + i * 2; // face offset * 2 triangles * 3 vertices * 2 uv coords
            uvs.push_back(cube_texcoords[uv_idx + 0]);
            uvs.push_back(cube_texcoords[uv_idx + 1]);
        }
    }
}

btype get_cube_or_neighbor(const Chunk& chunk, int x, int y, int z){
    // If within chunk bounds, return directly
    if (x >= 0 && x < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_WIDTH) {
        return chunk.get(x, y, z);
    }

    // Out of bounds—check neighboring chunks
    int neighbor_cx = chunk.cx;
    int neighbor_cy = chunk.cy;
    int neighbor_cz = chunk.cz;
    int local_x = x;
    int local_y = y;
    int local_z = z;

    // Adjust coordinates and neighbor chunk indices
    if (x < 0) {
        neighbor_cx--;
        local_x += CHUNK_WIDTH;
    } else if (x >= CHUNK_WIDTH) {
        neighbor_cx++;
        local_x -= CHUNK_WIDTH;
    }

    if (y < 0) {
        neighbor_cy--;
        local_y += CHUNK_HEIGHT;
    } else if (y >= CHUNK_HEIGHT) {
        neighbor_cy++;
        local_y -= CHUNK_HEIGHT;
    }

    if (z < 0) {
        neighbor_cz--;
        local_z += CHUNK_WIDTH;
    } else if (z >= CHUNK_WIDTH) {
        neighbor_cz++;
        local_z -= CHUNK_WIDTH;
    }

    // Try to get the neighboring chunk
	Chunk* neighbor = get_chunk_ptr(neighbor_cx, neighbor_cy, neighbor_cz);
	if(!neighbor)
		return _AIR;
	
	return neighbor->get(local_x, local_y, local_z);
}

const int MAX_BLOCK_TYPES = _COUNT;

// face culling & single chunk VBO
void buildChunkVBO(Chunk& chunk) {
    // Map per cube type using array for temporary storage
    std::vector<float> vertices[MAX_BLOCK_TYPES];
    std::vector<float> uvs[MAX_BLOCK_TYPES];

    try {
        // attempt allocation for each block type
        for(int i = 0; i < MAX_BLOCK_TYPES; i++){
            vertices[i].reserve(256); // optional, pre-reserve to reduce reallocs
            uvs[i].reserve(256);
        }
    } catch(const std::bad_alloc& e) {
        termio::cerr << "Failed to allocate memory for chunk arrays: " << e.what() << "\n";
        chunk.dirty = false; // mark as not generated
        return; // skip this chunk
    }

    // --- Fill vertices/UVs ---
    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_WIDTH; z++) {
                btype type = chunk.get(x, y, z);
                if (type == _AIR) continue;

                bool faces[6] = {true, true, true, true, true, true};

                // Face culling
                if (get_cube_or_neighbor(chunk, x - 1, y, z) != _AIR) faces[2] = false;
                if (get_cube_or_neighbor(chunk, x + 1, y, z) != _AIR) faces[3] = false;
                if (get_cube_or_neighbor(chunk, x, y - 1, z) != _AIR) faces[5] = false;
                if (get_cube_or_neighbor(chunk, x, y + 1, z) != _AIR) faces[4] = false;
                if (get_cube_or_neighbor(chunk, x, y, z - 1) != _AIR) faces[1] = false;
                if (get_cube_or_neighbor(chunk, x, y, z + 1) != _AIR) faces[0] = false;

                // Push faces
                push_cube_faces(vertices[static_cast<int>(type)],
                                uvs[static_cast<int>(type)],
                                chunk, x, y, z, faces);
            }
        }
    }

    // --- Upload VBOs per block type ---
    for (int i = 0; i < MAX_BLOCK_TYPES; i++) {
        std::vector<float>& verts = vertices[i];
        std::vector<float>& tex = uvs[i];
        if (verts.empty()) continue;

        btype type = static_cast<btype>(i);  // cast int -> btype for maps

        // Vertices
        GLuint& vbo = chunk.vbos[type];
        if (vbo == 0) glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

        // UVs
        GLuint& uvbo = chunk.uvbos[type];
        if (uvbo == 0) glGenBuffers(1, &uvbo);
        glBindBuffer(GL_ARRAY_BUFFER, uvbo);
        glBufferData(GL_ARRAY_BUFFER, tex.size() * sizeof(float), tex.data(), GL_STATIC_DRAW);

        // Vertex count
        chunk.vertexCounts[type] = verts.size() / 3;
    }

    chunk.dirty = false;
}

void update_chunks() {
    int meshes_per_frame = 2;  // limit work per frame

    for (auto& [key, chunk] : chunks) {
        if (chunk.dirty) {
            buildChunkVBO(chunk);
            meshes_per_frame--;

            if (meshes_per_frame <= 0)
                break;
        }
    }
}

// I SPEEK FOR DA TREES
void create_tree(Chunk& cur_chunk, int x, int z, int height){
    if(rand() % 100 < 1){ // ~1% chance to generate a tree at this spot
        // --- Trunk ---
        int trunkHeight = 5 + rand() % 3; // 4-6 blocks tall
        for(int y = height + 1; y <= height + trunkHeight && y < CHUNK_HEIGHT; y++){
            add_cube(cur_chunk, x, y, z, _TAL_TILE); // trunk block
        }

        // --- Leaves ---
        int leafStart = height + trunkHeight - 2;   // start 2 blocks below top
        int leafEnd   = height + trunkHeight - 1;   // 2-3 layers of leaves

        for(int y = leafStart; y <= leafEnd && y < CHUNK_HEIGHT; y++){
            int radius = leafEnd - y + 1; // smaller radius at top
            for(int dx = -radius; dx <= radius; dx++){
                for(int dz = -radius; dz <= radius; dz++){
                    int lx = x + dx;
                    int lz = z + dz;

                    // Skip if out of chunk bounds
                    if(lx < 0 || lx >= CHUNK_WIDTH || lz < 0 || lz >= CHUNK_WIDTH) continue;

                    // Roughly spherical
                    if(abs(dx) + abs(dz) <= radius){
                        if(get_cube(cur_chunk, lx, y, lz) == _AIR){
                            add_cube(cur_chunk, lx, y, lz, _TAL_TILE);
                        }
                    }
                }
            }
        }
    }
}

// chunk generation
#include <queue>
#include <set>

std::queue<std::tuple<int, int, int>> chunkGenerationQueue;
std::set<std::tuple<int, int, int>> generatedChunks;

void queue_chunk_generation(int ccx, int ccy, int ccz) {
    auto key = std::make_tuple(ccx, ccy, ccz);
    if (generatedChunks.find(key) == generatedChunks.end()) {
        chunkGenerationQueue.push(key);
        generatedChunks.insert(key);
    }
}

void create_chunk(int ccx, int ccy, int ccz){
    int startX = ccx * CHUNK_WIDTH;
    int startZ = ccz * CHUNK_WIDTH;

    Chunk& cur_chunk = get_chunk(ccx, ccy, ccz);

    for(int x = 0; x < CHUNK_WIDTH; x++){
        for(int z = 0; z < CHUNK_WIDTH; z++){
            int worldX = startX + x;
            int worldZ = startZ + z;

            int height = static_cast<int>(std::floor(perlin(worldX*0.02f, worldZ*0.02f)*CHUNK_HEIGHT));
            height = std::min(height, CHUNK_HEIGHT-1);

            for(int y = 0; y <= height; y++){
                if(y == height || y == height - 1){ 
                    add_cube(cur_chunk, x, y, z, _GROUND);
                }
                else {
                    add_cube(cur_chunk, x, y, z, _DIRT);
                }
            }
            
            create_tree(cur_chunk, x, z, height);
        }
    }

    cur_chunk.dirty = true;
    
    // Queue neighbors for generation (don't generate them directly)
	queue_chunk_generation(ccx + 1, ccy, ccz);
	queue_chunk_generation(ccx - 1, ccy, ccz);
	queue_chunk_generation(ccx, ccy, ccz + 1);
	queue_chunk_generation(ccx, ccy, ccz - 1);
}

void process_chunk_generation_queue() {
    // Process a limited number per frame to avoid lag spikes
    int chunks_to_process = 4;  // Adjust based on performance
    
    while (!chunkGenerationQueue.empty() && chunks_to_process > 0) {
        auto [ccx, ccy, ccz] = chunkGenerationQueue.front();
        chunkGenerationQueue.pop();
        
        create_chunk(ccx, ccy, ccz);
        chunks_to_process--;
    }
}

// Create the seed
int init_world(int seed){
	if(seed == 0) seed = randomi("rd");
    initPerlin(seed);
    return seed;
}
