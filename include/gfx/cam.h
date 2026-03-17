#ifndef CAM_H
#define CAM_H

struct camera{
	// Position
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	
	// Angles
	float yaw = 0.f;
	float pitch = 0.f;
};

extern camera cam;

#endif
