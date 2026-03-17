#include <GL/glew.h>
#include "gfx/text.h"
#include "gfx/textures.h"

// vertices for the quad
GLfloat quad_vertices[] = {
	-1, -1, 0,
	1, -1, 0,
	1, 1, 0,
	-1, 1, 0
};

GLfloat quad_texcoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};

// VBOs
GLuint quadVBO;
GLuint quadUV;

// Initialize the VBOs
void init_quadvbo(){
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
}

void init_quaduv(){
	glGenBuffers(1, &quadUV);
	glBindBuffer(GL_ARRAY_BUFFER, quadUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_texcoords), quad_texcoords, GL_STATIC_DRAW);
}

void draw_quad(Texture* tex){
	if(!tex) return;

	tex->Bind(GL_TEXTURE0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glVertexPointer(3, GL_FLOAT, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, quadUV);
	glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);

	glDrawArrays(GL_QUADS, 0, 24);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_quad_special(Texture* tex, float x, float y, float z, float u0, float u1, float v0, float v1, float size) {
    if (!tex) return;

    tex->Bind(GL_TEXTURE0);

    // Define the vertices for the quad based on position and size
    float halfSize = size / 2;

    // The vertex data
	float vertices[12] = {
	    x - halfSize, y - halfSize, z,   // Bottom-left
	    x - halfSize, y + halfSize, z,   // Top-left
	    x + halfSize, y - halfSize, z,   // Bottom-right
	    x + halfSize, y + halfSize, z    // Top-right
	};

    // The texture coordinates for the quad
    float texCoords[12] = {
		u0, v1,
		u1, v1,
		u0, v0,
		u1, v0
    };

    // Binding and texturing process
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Update buffer data with new vertices
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, quadUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);

    // Draw the quad
    glDrawArrays(GL_QUADS, 0, 4);  // Use TRIANGLE_STRIP for two triangles

    // Clean up states
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
