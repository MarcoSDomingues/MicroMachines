#include "OrtogonalCamera.h"
#include "AVTmathLib.h"

OrtogonalCamera::OrtogonalCamera(float left, float right, float bottom, float top, float near, float far)
{
	_left = left;
	_right = right;
	_bottom = bottom;
	_top = top;
	_near = near;
	_far = far;
}


void OrtogonalCamera::update(float ratio) {
	lookAt(0, 0, 0, 0, 0, 0, 0, 1, 0);
	if (ratio > 1) {
	ortho(-5 * ratio, 5 * ratio, -5, 5, -10, 10);
	}
	else {
	ortho(-5 , 5, -5 / ratio, 5 / ratio, -10, 10);
	}
}