#include "world/map.h"
#include "gfx/cam.h"
#include "out.h"
#include <cmath>

struct Vec3 { float x, y, z; };

// Normalize
Vec3 normalize(const Vec3& v){
    float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    return { v.x/len, v.y/len, v.z/len };
}

// Get direction camera is facing
Vec3 getCameraDir(camera& cam){
    float yawRad = cam.yaw * M_PI / 180.f;
    float pitchRad = cam.pitch * M_PI / 180.f;

    Vec3 dir;
    dir.x = cosf(pitchRad) * cosf(yawRad);
    dir.y = sinf(pitchRad);
    dir.z = cosf(pitchRad) * sinf(yawRad);
    return dir;
}

// Raycast along camera
bool raycast(Vec3 origin, Vec3 dir, float maxDist, Vec3& hitPos, Vec3& normal){
    dir = normalize(dir);
    Vec3 pos = origin;
    float step = 0.1f;

    for(float t = 0; t < maxDist; t += step){
        pos.x = origin.x + dir.x * t;
        pos.y = origin.y + dir.y * t;
        pos.z = origin.z + dir.z * t;

        int bx = (int)floorf(pos.x);
        int by = (int)floorf(pos.y);
        int bz = (int)floorf(pos.z);

        int cx = bx / CHUNK_WIDTH;
        int cy = by / CHUNK_HEIGHT;
        int cz = bz / CHUNK_WIDTH;

        int lx = bx % CHUNK_WIDTH;
        int ly = by % CHUNK_HEIGHT;
        int lz = bz % CHUNK_WIDTH;

        if(lx < 0){ lx += CHUNK_WIDTH; cx--; }
        if(ly < 0){ ly += CHUNK_HEIGHT; cy--; }
        if(lz < 0){ lz += CHUNK_WIDTH; cz--; }

        Chunk* chunk = get_chunk_ptr(cx, cy, cz);
        if(!chunk) continue;

        btype block = chunk->get(lx, ly, lz);
        if(block != _AIR){
            hitPos = { (float)bx, (float)by, (float)bz };

            // Approximate which face was hit
            normal = {0,0,0};
            float fx = pos.x - bx;
            float fy = pos.y - by;
            float fz = pos.z - bz;

            if(fx < 0.001f) normal.x = -1; else if(fx > 0.999f) normal.x = 1;
            if(fy < 0.001f) normal.y = -1; else if(fy > 0.999f) normal.y = 1;
            if(fz < 0.001f) normal.z = -1; else if(fz > 0.999f) normal.z = 1;

            return true;
        }
    }

    return false;
}

// Destroy block at crosshair
void destroyBlock(camera& cam){
    Vec3 hit, normal;
    if(raycast({cam.x, cam.y + 1.6f, cam.z}, getCameraDir(cam), 6.f, hit, normal)){
        int bx = (int)hit.x;
        int by = (int)hit.y;
        int bz = (int)hit.z;

        int cx = bx / CHUNK_WIDTH;
        int cy = by / CHUNK_HEIGHT;
        int cz = bz / CHUNK_WIDTH;

        int lx = bx % CHUNK_WIDTH;
        int ly = by % CHUNK_HEIGHT;
        int lz = bz % CHUNK_WIDTH;

        if(lx < 0){ lx += CHUNK_WIDTH; cx--; }
        if(ly < 0){ ly += CHUNK_HEIGHT; cy--; }
        if(lz < 0){ lz += CHUNK_WIDTH; cz--; }

        Chunk* chunk = get_chunk_ptr(cx, cy, cz);
        if(!chunk) return;

        add_cube(*chunk, lx, ly, lz, _AIR);
        chunk->dirty = true;
    }
}

// Place block next to targeted block
void placeBlock(camera& cam, btype blockType){
    Vec3 hit, normal;
    if(raycast({cam.x, cam.y, cam.z}, getCameraDir(cam), 6.f, hit, normal)){
        int px = (int)(hit.x + normal.x);
        int py = (int)(hit.y + normal.y);
        int pz = (int)(hit.z + normal.z);

        int cx = px / CHUNK_WIDTH;
        int cy = py / CHUNK_HEIGHT;
        int cz = pz / CHUNK_WIDTH;

        int lx = px % CHUNK_WIDTH;
        int ly = py % CHUNK_HEIGHT;
        int lz = pz % CHUNK_WIDTH;

        if(lx < 0){ lx += CHUNK_WIDTH; cx--; }
        if(ly < 0){ ly += CHUNK_HEIGHT; cy--; }
        if(lz < 0){ lz += CHUNK_WIDTH; cz--; }

        Chunk* chunk = get_chunk_ptr(cx, cy, cz);
        if(!chunk) return;

        add_cube(*chunk, lx, ly, lz, blockType);
        chunk->dirty = true;
        
		Vec3 hit, normal;
		bool h = raycast({cam.x, cam.y + 1.6f, cam.z}, getCameraDir(cam), 6.f, hit, normal);
		termio::info << "Raycast hit: " << h << " pos: " << hit.x << "," << hit.y << "," << hit.z << "\n";

    }
}
