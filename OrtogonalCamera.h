#pragma once
#include "Camera.h"
class OrtogonalCamera : public Camera {
	float _left;
	float _right;
	float _bottom;
	float _top;

public:
	OrtogonalCamera(float left, float right, float bottom, float top, float near, float far);
	void update(float ratio);
};