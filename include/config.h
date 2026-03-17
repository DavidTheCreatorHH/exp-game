#ifndef CONFIG_H
#define CONFIG_H

// a simple configuration header file
// Instead of hunting for the constants, just edit them here
// before building the project. simple and easy!

const int VIEW_DISTANCE_X = 2;
const int VIEW_DISTANCE_Z = 2;

const int CHUNKS_X = 1;
const int CHUNKS_Y = 1;
const int CHUNKS_Z = 1;

const int CHUNK_WIDTH =  32;
const int CHUNK_HEIGHT = 64;

const float MOVEMENT_SPEED = 6.f;
const float FALLING_SPEED = 24.4f;
const float JUMP_HEIGHT = 7.f;

#endif
