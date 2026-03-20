#define GLFW_INCLUDE_NONE	// I cannot describe the suffering this caused
// OpenGL headers
#include "glew.h"
#include "glfw3.h"

// cxx headers
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

// game headers
#include "gfx/graphics.h"
#include "init.h"
#include "gfx/cam.h"
#include "world/map.h"
#include "gfx/cinf.h"
#include "player/ctrls.h"
#include "out.h"

// width & height
int WIDTH = 840;
int HEIGHT = 640;

// game name
std::string Name = "Exp";

bool paused = false;

// create camera
camera cam;

// version
const char* Version_pref = "dv_";
const int V_MINOR = 4;
const int V_MAJOR = 1;
const int V_RELEASE = 0;

std::string Version =	// Construct the version string
    Version_pref +
    std::to_string(V_RELEASE) + "." +
    std::to_string(V_MAJOR) + "." +
    std::to_string(V_MINOR);

// mouse stuff
float last_mouse_x = WIDTH / 2.0f;
float last_mouse_y = HEIGHT / 2.0f;
bool first_mouse = true;
const float MOUSE_SENSITIVITY = 0.1f;

// resize the window (WARNING: It distors everything)
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
}

// for mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(!paused){
		if(first_mouse){
			last_mouse_x = xpos;
			last_mouse_y= ypos;
			first_mouse = false;
		}

		float xoffset = xpos - last_mouse_x;
		float yoffset = last_mouse_y - ypos;
		last_mouse_x = xpos;
		last_mouse_y = ypos;

		xoffset *= MOUSE_SENSITIVITY;
		yoffset *= -MOUSE_SENSITIVITY;

		cam.yaw += xoffset;
		cam.pitch += yoffset;

		if (cam.pitch > 89.0f) cam.pitch = 89.0f;
		if (cam.pitch < -89.0f) cam.pitch = -89.0f;
	}
}

// for keys that can only be pressed once
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(action == GLFW_PRESS){
		if(!paused){
			// quit
			if(key == GLFW_KEY_ESCAPE){
				glfwSetWindowShouldClose(window, 1);
			}
			// enable noclip
			if(key == GLFW_KEY_N){
				noclip = !noclip;
			}
			// reset camerar position
			if(key == GLFW_KEY_B){
				cam.x= 0;
				cam.y = 30.f;
				cam.z = 0;
			}
		}

		// pause the game
		if(key == GLFW_KEY_P){
			if(paused){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				paused = false;
				first_mouse = true;
				glfwSetWindowTitle(window, (Name + " " + Version + " ~ [Running]").c_str());
			}
			else{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				paused = true;
			}
		}
	}
}

// check if enough memory is avaiable
size_t get_mem(){
#if defined(__linux__)
    std::ifstream mem("/proc/meminfo");
    std::string line;
    while (std::getline(mem, line)) {
        if(line.find("MemAvailable:") == 0){
            size_t kb;
            sscanf(line.c_str(), "MemAvailable: %zu kB", &kb);
            return kb * 1024; // convert KB -> bytes
        }
    }
    return 0;
#elif defined(_WIN32)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return static_cast<size_t>(status.ullAvailPhys);
#elif defined(__APPLE__) || defined(__FreeBSD__)
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    vm_statistics_data_t vmstat;
    if(host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count) != KERN_SUCCESS)
        return 0;
    size_t free_mem = (vmstat.free_count + vmstat.inactive_count) * sysconf(_SC_PAGESIZE);
    return free_mem;
#else
    return 0;
#endif
}

int seed = 0;
char* err_string = (char*)"";

// Create the game window
GLFWwindow* create_game(){
	// Before running the game, check available memory
	size_t bytes_per_block = sizeof(btype); // usually 1 byte
	size_t blocks_per_chunk = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH;
	size_t chunk_memory = blocks_per_chunk * bytes_per_block * 4;
	if(get_mem() < chunk_memory){
		termio::cerr << "ERR: Not enough memory for chunks!\nTIP: Close any unused programs";
		std::exit(1);
		return nullptr;
	}
	std::string winname = Name + " " + Version + " ~ [Running]";
	GLFWwindow* game = glfwCreateWindow(WIDTH, HEIGHT, winname.c_str(), NULL, NULL);
	
	if(!game){
        std::cerr << "Failed to create game window!\n";
        return nullptr;
    }
	
	glfwMakeContextCurrent(game);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(GLEW_OK != err){
		std::cerr << "Failed to init GLEW for game context: " << glewGetErrorString(err);
		return nullptr;
	}
	
	// set callbacks
	glfwSetFramebufferSizeCallback(game, framebuffer_size_callback);
	glfwSetKeyCallback(game, key_callback);
	glfwSetCursorPosCallback(game, mouse_callback);
	glfwSetInputMode(game, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// init any stuff we need
	init_cam();
	init_GL(WIDTH, HEIGHT, render_dist);
	init_graphics();

	// create seed
	termio::info << "World Seed: " << init_world(seed) << '\n';

	// load chunks (first frame)
	load_chunks(cam.x, cam.z);

	// move the player ON to to the ground
	init_y_cam(cam.y);

	return game;	// return window
}

int main(){
    termio::set_log_level(log_level::info);	// because i needa know everything

	// Initialize everything

    if(!glfwInit()){	// GLFW
        const char* glfwerr;
        glfwGetError(&glfwerr);
        termio::cerr << "CERR: Failed to intialize GLFW!: " << glfwerr;
        return 1;
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_VERSION_MINOR, 1);

    // create the game window via create_game
    GLFWwindow* game = create_game();
    if(!game){
        termio::cerr << "CERR: Failed to create game window!\n";
        glfwTerminate();
        return 2;
    }

    termio::info << "<< " << Name << " Version: " << Version << " >>\n";
    //termio::info << Changelog;	// Currently NOT doing that

    // main game loop
    while(!glfwWindowShouldClose(game)){
        glfwPollEvents();

        if(!paused){
            glfwMakeContextCurrent(game);

			// block manipulation (Currently does NOT work)
            // destroy block
			if(glfwGetMouseButton(game, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
				destroyBlock(cam);
			}

			// place block
			if(glfwGetMouseButton(game, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
				placeBlock(cam, _DIRT); // or your selected block type
			}
            
            update(game);
            render();
            glfwSwapBuffers(game);
        }
        else { // Paused
        	// idfk it doesn't work
            glfwMakeContextCurrent(game);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            render();

            glDisable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, WIDTH, HEIGHT, 0, -100, 100);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glfwSetWindowTitle(game, (Name + " " + Version + " ~ [Paused]").c_str());

            glColor4f(0.2f, 0.2f, 0.2f, 0.7f);
            glBegin(GL_QUADS);
            glVertex2f(-WIDTH, -HEIGHT);
            glVertex2f(WIDTH, -HEIGHT);
            glVertex2f(WIDTH, HEIGHT);
            glVertex2f(-WIDTH, HEIGHT);
            glEnd();
            glColor4f(1.f, 1.f, 1.f, 1.f);

            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glEnable(GL_DEPTH_TEST);

            glfwSwapBuffers(game);
        }
    }

	termio::info << '\n';

    // clean everything up
    glfwDestroyWindow(game);
    glfwTerminate();

    return 0;
}
