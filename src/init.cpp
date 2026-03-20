#include "glew.h"
#include "init.h"
#include "gfx/cube.h"
#include "gfx/cinf.h"
#include "world/map.h"
#include "gfx/quad.h"
#include "gfx/graphics.h"
#include "world/chunks.h"
#include "gfx/cam.h"
#include "config.h"
#include <math.h>

// init OpenGL
void init_GL(int WIDTH, int HEIGHT, float renderDist){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    // Setup a perspective projection matrix
    float aspectRatio = (float)WIDTH / HEIGHT;
    float fov = 45.0f;
    float near = 0.1f;
    float far = renderDist;

    // Calculate projection matrix
    float f = 1.0f / tan(fov * 0.5f * M_PI / 180.0f);
    float m[16] = {
        f / aspectRatio, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near) / (near - far), -1,
        0, 0, (2 * far * near) / (near - far), 0
    };

    glMultMatrixf(m);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// put the player on top of the map
void init_y_cam(float& cam_y){
	long long key = chunkKey(0, 0, 0);
	Chunk& chunk = chunks[key];  // or: Chunk& chunk = chunks[key];

	chunk.cx = 0;
	chunk.cy = 0;
	chunk.cz = 0;

	Chunk first_chunk = chunk;
    for (int i = 0; i < 256; i++) {
        btype* cube = get_cube(first_chunk, cam.x, i, cam.z);
        if (cube == nullptr) {  // check if pointer is null
            cam_y = (((float)i + 2) * 2) + player_height;
            break;
        }
    }
    
    // default
    cam_y = 100.f;
}

// Init graphics
void init_graphics(){
	// Enable some GL stuff
	glEnable(GL_TEXTURE_2D);	// textures
	glEnable(GL_CULL_FACE);		// face culling
	glEnable(GL_DEPTH_TEST);	// depth testing
	glEnable(GL_BLEND);			// transparency
	glEnable(GL_FOG);			// fog
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	
	// Maximum distance from player to corner of view rectangle
	float maxViewDistance = sqrt(
		(VIEW_DISTANCE_X * CHUNK_WIDTH) * (VIEW_DISTANCE_X * CHUNK_WIDTH) +
		(VIEW_DISTANCE_X * CHUNK_WIDTH) * (VIEW_DISTANCE_X * CHUNK_WIDTH)
	);
	
	GLfloat fogColor[4] = {0.5f, 0.7f, 1.0f, 1.0f};
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, maxViewDistance * 0.3f); // start fading 40% of max
	glFogf(GL_FOG_END, maxViewDistance);           // fully opaque at max
	glHint(GL_FOG_HINT, GL_FASTEST);
	
	// init the VBOs
	init_cubevbo();
	init_cube_uvbo();

	// quad
	init_quadvbo();
	init_quaduv();

	// Load the textures

	RegisterTextures();
	LoadAllTextures();
}

// init camera
void init_cam(){
    cam.x = CHUNK_WIDTH * CHUNKS_X;
    //cam.y = 0.f;
    cam.z = CHUNK_WIDTH * CHUNKS_Z;

    cam.pitch = 0.f;
    cam.yaw = 0.f;
}
