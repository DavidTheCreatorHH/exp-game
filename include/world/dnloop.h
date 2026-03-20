#ifndef DNLOOP_H
#define DNLOOP_H
#include "glew.h"

// Brightness of the world
extern GLfloat wBrightness;

extern float lerp(float a, float b, float t);

float change_color(float deltaTime);

#endif
