#include <GL/glew.h>
#include <cmath>
#include "gfx/frustum.h"

// Frustum stuff
// the sides for the frustum
enum Frustum_side{
	RIGHT = 0,
	LEFT = 1,
	TOP = 2,
	BOTTOM = 3,
	BACK = 4,
	FRONT = 5
};

// plane data thingy
enum Plane_data{
	A = 0,
	B = 1,
	C = 2,
	D = 3
};

// Normalizer the plane
void Normalize_plane(float frustum[6][4], int side){
	float magnitude = (float)sqrt(	frustum[side][A] * frustum[side][A] +	
									frustum[side][B] * frustum[side][B] +
									frustum[side][C] * frustum[side][C]	);

	frustum[side][A] /= magnitude;
	frustum[side][B] /= magnitude;
	frustum[side][C] /= magnitude;
	frustum[side][D] /= magnitude;
}

// Get the current frustum
void CFrustum::CalculateFrustum(){
	float proj[16];
	float modl[16];
	float clip[16];

	glGetFloatv(GL_PROJECTION_MATRIX, proj);

	glGetFloatv(GL_MODELVIEW_MATRIX, modl);

	// MATH
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];


	// Calculate the frustum things
	// RIGHT
	m_Frustum[RIGHT][A] = clip[3] - clip[0];
	m_Frustum[RIGHT][B] = clip[7] - clip[4];
	m_Frustum[RIGHT][C] = clip[11] - clip[8];
	m_Frustum[RIGHT][D] = clip[15] - clip[12];
	Normalize_plane(m_Frustum, RIGHT);

	// LEFT
	m_Frustum[LEFT][A] = clip[3] + clip[0];
	m_Frustum[LEFT][B] = clip[7] + clip[4];
	m_Frustum[LEFT][C] = clip[11] + clip[8];
	m_Frustum[LEFT][D] = clip[15] + clip[12];
	Normalize_plane(m_Frustum, LEFT);

	// BOTTOM
	m_Frustum[BOTTOM][A] = clip[3] + clip[1];
	m_Frustum[BOTTOM][B] = clip[7] + clip[5];
	m_Frustum[BOTTOM][C] = clip[11] + clip[9];
	m_Frustum[BOTTOM][D] = clip[15] + clip[13];
	Normalize_plane(m_Frustum, BOTTOM);

	// TOP
	m_Frustum[TOP][A] = clip[3] - clip[1];
	m_Frustum[TOP][B] = clip[7] - clip[5];
	m_Frustum[TOP][C] = clip[11] - clip[9];
	m_Frustum[TOP][D] = clip[15] - clip[13];
	Normalize_plane(m_Frustum, TOP);

	// BACK (far)
	m_Frustum[BACK][A] = clip[3] - clip[2];
	m_Frustum[BACK][B] = clip[7] - clip[6];
	m_Frustum[BACK][C] = clip[11] - clip[10];
	m_Frustum[BACK][D] = clip[15] - clip[14];
	Normalize_plane(m_Frustum, BACK);

	// FRONT (near)
	m_Frustum[FRONT][A] = clip[3] + clip[2];
	m_Frustum[FRONT][B] = clip[7] + clip[6];
	m_Frustum[FRONT][C] = clip[11] + clip[10];
	m_Frustum[FRONT][D] = clip[15] + clip[14];
	Normalize_plane(m_Frustum, FRONT);
}

// check if a point is in frustum
bool CFrustum::PointInFrustum(float x, float y, float z){
	for(int i = 0; i < 6; i++){
		if(m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= 0){
			return false;
		}
	}

	return true;
}

// check if a cube is in frustum
bool CFrustum::CubeInFrustum(float x, float y, float z, float size){
	for(int i = 0; i < 6; i++){
		// check the 8 corners of the AABB
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0) continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0) continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0) continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0) continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0) continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0) continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0) continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0) continue;

		// if at least one point is in the frustum
		return false;
	}

	// if all points are outside the frustum
	return true;
}

// check if a box is in frustum
bool CFrustum::BoxInFrustum(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax) {
    for (int i = 0; i < 6; i++) {
        // check all 8 corners of the AABB
        if (m_Frustum[i][A] * xMin + m_Frustum[i][B] * yMin + m_Frustum[i][C] * zMin + m_Frustum[i][D] >= 0) continue;
        if (m_Frustum[i][A] * xMax + m_Frustum[i][B] * yMin + m_Frustum[i][C] * zMin + m_Frustum[i][D] >= 0) continue;
        if (m_Frustum[i][A] * xMin + m_Frustum[i][B] * yMax + m_Frustum[i][C] * zMin + m_Frustum[i][D] >= 0) continue;
        if (m_Frustum[i][A] * xMax + m_Frustum[i][B] * yMax + m_Frustum[i][C] * zMin + m_Frustum[i][D] >= 0) continue;
        if (m_Frustum[i][A] * xMin + m_Frustum[i][B] * yMin + m_Frustum[i][C] * zMax + m_Frustum[i][D] >= 0) continue;
        if (m_Frustum[i][A] * xMax + m_Frustum[i][B] * yMin + m_Frustum[i][C] * zMax + m_Frustum[i][D] >= 0) continue;
        if (m_Frustum[i][A] * xMin + m_Frustum[i][B] * yMax + m_Frustum[i][C] * zMax + m_Frustum[i][D] >= 0) continue;
        if (m_Frustum[i][A] * xMax + m_Frustum[i][B] * yMax + m_Frustum[i][C] * zMax + m_Frustum[i][D] >= 0) continue;

        // if all 8 points are outside this plane, the box is outside the frustum
        return false;
    }
    return true;
}
