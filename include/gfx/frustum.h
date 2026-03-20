#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "glew.h"

class CFrustum{
	public:
		void CalculateFrustum();
		bool PointInFrustum(float x, float y, float z);
		bool CubeInFrustum(float x, float y, float z, float size);
		bool BoxInFrustum(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax);

	private:
		float m_Frustum[6][4];
};

#endif
