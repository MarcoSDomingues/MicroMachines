#pragma once
#include "Vector3.h"
class Camera
{
protected:
	Vector3 _up;
	Vector3* _at;
	float _near;
	float _far;

public:
	~Camera();
	virtual void update(float ratio);
	void setAt(float x, float y, float z);

};