#pragma once
#include "Camera.h"
class PerspectiveCamera : public Camera {
	float _fov;

public:
	PerspectiveCamera(float fov, float near, float far);
	void update(float ratio);
};