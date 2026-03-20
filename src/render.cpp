// because GLFW is evil
#define GLFW_INCLUDE_NONE
// cxx headers
#include <iostream>
// GL headers
#include "glew.h"
#include "glfw3.h"
// game headers
#include "gfx/graphics.h"
#include "gfx/frustum.h"
#include "gfx/text.h"
#include "world/chunks.h"
#include "phys/collison.h"
#include "gfx/cube.h"
#include "init.h"
#include "world/dnloop.h"
#include "gfx/cinf.h"
#include "gfx/cam.h"
#include "player/ctrls.h"
#include "out.h"
#include "config.h"

// render distance (in OpenGL terms)
int const render_dist = 1000;

// player stuff
const float player_height = 1.2f;
bool noclip = false;

// frustum culling
CFrustum gFrustum;

void day_night_color(const Cube& c, float gBrightness){
	if(c.type == TEX_LAVA){
		// Lava always glows
		float r = std::min(1.0f, (c.r / 255.0f) * c.light);
		float g = std::min(1.0f, (c.g / 255.0f) * c.light);
		float b = std::min(1.0f, (c.b / 255.0f) * c.light);
		glColor3f(r, g, b);
	}
	else{
		// Other blocks should dim when night
		float r = std::min(1.0f, (c.r / 255.0f) * c.light * gBrightness);
		float g = std::min(1.0f, (c.g / 255.0f) * c.light * gBrightness);
		float b = std::min(1.0f, (c.b / 255.0f) * c.light * gBrightness);
		glColor3f(r, g, b);
	}
}

Texture* get_tex(btype type){
	if(type == _GROUND)				return Textures[TEX_GROUND].get();
	else if(type == _LEGACY_GROUND)	return Textures[TEX_GROUND_OLD].get();
	else if(type == _DIRT)			return Textures[TEX_DIRT].get();
	else if(type == _LAVA)			return Textures[TEX_LAVA].get();
	else if(type == _COLD_LAVA)		return Textures[TEX_COLD_LAVA].get();
	else if(type == _OBSIDIAN)		return Textures[TEX_OBSIDIAN].get();
	else if(type == _TASR)			return Textures[TEX_TASR].get();
	else if(type == _EREL_CRYSTAL)	return Textures[TEX_EREL_CRYSTAL].get();
	else if(type == _TAL_TILE)		return Textures[TEX_TAL_TILE].get();
	else if(type == _TEST)			return Textures[TEX_TEST].get();
	
	return nullptr;
}

void draw_map(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	for (auto& [key, chunk] : chunks) {
		// Loop over cube types in this chunk
		for (auto& [type, vbo] : chunk.vbos){
			glBindTexture(GL_TEXTURE_2D, get_tex(type)->getID());

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexPointer(3, GL_FLOAT, 0, nullptr);

			glBindBuffer(GL_ARRAY_BUFFER, chunk.uvbos[type]);
			glTexCoordPointer(2, GL_FLOAT, 0, nullptr);

			glDrawArrays(GL_TRIANGLES, 0, chunk.vertexCounts[type]);
		}
	}

	load_chunks(cam.x, cam.z);
	unload_chunks(cam.x, cam.z);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// DEPRECATED!!
// a function for drawing a cube
void draw_cube(Texture* Current_tex){
	if(!Current_tex) return;

	Current_tex->Bind(GL_TEXTURE0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glVertexPointer(3, GL_FLOAT, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, cubeUV);
	glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);

	glDrawArrays(GL_QUADS, 0, 24);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

double Last_time;
double Delta_time;
const float Step = 0.2f;

float Move_Incremental(float& pos, float delta){
	if(delta == 0.0f) return 0.f;
	float step = (delta > 0) ? Step : -Step;
	float remaining = delta;

	while(fabs(remaining) > fabs(step)){
		pos += step;

		if(isColliding(cam.x, cam.y, cam.z, player_height)){
			pos -= step;
			return 1.f;
		}

		remaining -= step;
	}

	pos += remaining;
	if(isColliding(cam.x, cam.y, cam.z, player_height)){
		pos -= remaining;
	}

	float ahh = pos;

	return ahh;
}

void UpdateFPS(){
	static double FPSlast_time = 0.0;
	static int FPSFrames = 0;
	static double FPSValue = 0.0;
	double current = glfwGetTime();
	FPSFrames++;

	if(current - FPSlast_time >= 1.0){
		FPSValue = FPSFrames / (current - FPSlast_time);
		FPSFrames = 0;
		FPSlast_time = current;

		termio::info << "FPS > " << FPSValue << '\r';
		std::cout.flush();
	}
}

void update(GLFWwindow* window) {
    double Cur_time = glfwGetTime();
    Delta_time = Cur_time - Last_time;
    Last_time = Cur_time;

	UpdateFPS();

	static float jump_velocity = 0.f;
	static bool is_jumping = false;
	static bool last_y = 0.f;

	last_y = cam.y;

    // Compute movement deltas
    float moveSpeed = MOVEMENT_SPEED * Delta_time;
    float dx = 0.f, dy = 0.f, dz = 0.f;

	float yaw_rad = cam.yaw * M_PI / 180.0f;

	if(!noclip){
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		    dx += moveSpeed * sin(yaw_rad);
		    dz -= moveSpeed * cos(yaw_rad);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		    dx -= moveSpeed * sin(yaw_rad);
		    dz += moveSpeed * cos(yaw_rad);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		    dx -= moveSpeed * cos(yaw_rad);
		    dz -= moveSpeed * sin(yaw_rad);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		    dx += moveSpeed * cos(yaw_rad);
		    dz += moveSpeed * sin(yaw_rad);
		}

		if(!is_jumping){
			dy -= FALLING_SPEED * Delta_time;
		}
		
		// for block placing/destroying
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
			destroyBlock(cam);
		}
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
			placeBlock(cam, _DIRT); // or currently selected block
		}

		static bool space_pressed = false;

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		    if(!is_jumping && !space_pressed){
		    	dy = 0.f;
		        jump_velocity = JUMP_HEIGHT;
		        is_jumping = true;
		    }
		    space_pressed = true; // Should only be set if a jump was initiated
		}
		else {
		    space_pressed = false; // Reset if the key is released
		}

		if (is_jumping) {
		    dy += jump_velocity * Delta_time;
		    jump_velocity -= FALLING_SPEED * Delta_time;

		    float prev_cam_y = static_cast<float>(cam.y);

		    // attempt movement
		    Move_Incremental(cam.y, dy);

		    float new_cam_y = static_cast<float>(cam.y);

		    // LANDING CHECK
		    if (jump_velocity <= 0.0f && fabs(new_cam_y - prev_cam_y) < 0.01f){
		        is_jumping = false;
		        jump_velocity = 0.f;
		        dy = 0.f;
		    }
		}

		// Handle collisions per axis
		Move_Incremental(cam.x, dx);
		Move_Incremental(cam.y, dy);
		Move_Incremental(cam.z, dz);
	}

	if(noclip){

		// Movement
		moveSpeed *= 5;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		    dx += moveSpeed * sin(yaw_rad);
		    dz -= moveSpeed * cos(yaw_rad);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		    dx -= moveSpeed * sin(yaw_rad);
		    dz += moveSpeed * cos(yaw_rad);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		    dx -= moveSpeed * cos(yaw_rad);
		    dz -= moveSpeed * sin(yaw_rad);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		    dx += moveSpeed * cos(yaw_rad);
		    dz += moveSpeed * sin(yaw_rad);
		}
		
	    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) dy += moveSpeed;
	    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) dy -= moveSpeed;
	
		cam.x += dx;
		cam.y += dy;
		cam.z += dz;
	}
}

void render(){
	// clear OpenGL buffers
	glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // sky blue :]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// handle camera movement
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRotatef(cam.pitch, 1.0f, 0.0f, 0.0f); // X-axis
	glRotatef(cam.yaw, 0.0f, 1.0f, 0.0f); // Y-axis
	glTranslatef(-cam.x, -cam.y, -cam.z);

	// draw the world
	gFrustum.CFrustum::CalculateFrustum();
	draw_map();

	glPopMatrix();
	
	// UI (currently not used)
	/*
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// set up ortographic projection
	float aspect = (float)WIDTH / HEIGHT;
	glOrtho(0.f, 1.f, 0, 1, -100.f, 100.f);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glTranslatef(0.f, 0.f, 1.f);
	glColor3f(0.f, 0.f, 0.f);
	glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, 1.0f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(1.f, 0.f, 1.0f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.5f, 1.f, 1.0f);
	glEnd();
	glColor3f(1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();*/
}
