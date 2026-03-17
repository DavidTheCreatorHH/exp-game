#include "gfx/cube.h"
#include <stdbool.h>
#include "gfx/textures.h"

// VBO and UV for the cube object
GLuint cubeVBO;
GLuint cubeUV;

// vertices for the cube
float cube_vertices[6*2*3*3] = {
    // front face
    0,0,1,  1,0,1,  1,1,1,   // triangle 1
    0,0,1,  1,1,1,  0,1,1,   // triangle 2

    // back face
    1,0,0,  0,0,0,  0,1,0,   // triangle 1
    1,0,0,  0,1,0,  1,1,0,   // triangle 2

    // left face
    0,0,0,  0,0,1,  0,1,1,   // triangle 1
    0,0,0,  0,1,1,  0,1,0,   // triangle 2

    // right face
    1,0,1,  1,0,0,  1,1,0,   // triangle 1
    1,0,1,  1,1,0,  1,1,1,   // triangle 2

    // top face
    0,1,1,  1,1,1,  1,1,0,   // triangle 1
    0,1,1,  1,1,0,  0,1,0,   // triangle 2

    // bottom face
    0,0,0,  1,0,0,  1,0,1,   // triangle 1
    0,0,0,  1,0,1,  0,0,1    // triangle 2
};

// texture coor
GLfloat cube_texcoords[] = {
    // front face
    0.0f,0.0f,  1.0f,0.0f,  1.0f,1.0f,  // triangle 1
    0.0f,0.0f,  1.0f,1.0f,  0.0f,1.0f,  // triangle 2

    // back face
    0.0f,0.0f,  1.0f,0.0f,  1.0f,1.0f,  // triangle 1
    0.0f,0.0f,  1.0f,1.0f,  0.0f,1.0f,  // triangle 2

    // left face
    0.0f,0.0f,  1.0f,0.0f,  1.0f,1.0f,  // triangle 1
    0.0f,0.0f,  1.0f,1.0f,  0.0f,1.0f,  // triangle 2

    // right face
    0.0f,0.0f,  1.0f,0.0f,  1.0f,1.0f,  // triangle 1
    0.0f,0.0f,  1.0f,1.0f,  0.0f,1.0f,  // triangle 2

    // top face
    0.0f,0.0f,  1.0f,0.0f,  1.0f,1.0f,  // triangle 1
    0.0f,0.0f,  1.0f,1.0f,  0.0f,1.0f,  // triangle 2

    // bottom face
    0.0f,0.0f,  1.0f,0.0f,  1.0f,1.0f,  // triangle 1
    0.0f,0.0f,  1.0f,1.0f,  0.0f,1.0f   // triangle 2
};

void init_cubevbo(){
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
}

void init_cube_uvbo(){
	glGenBuffers(1, &cubeUV);
	glBindBuffer(GL_ARRAY_BUFFER, cubeUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);
}
