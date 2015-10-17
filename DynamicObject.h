#pragma once

#include "GameObject.h"

class DynamicObject : public GameObject {

protected:

	float _speed = 0;
	float _acceleration = 0.0f;

public:

	void setSpeed(float acceleration);
	void setAcceleration(float speed);
	float getSpeed();
	float getAcceleration();

	virtual void update(double delta_t) = 0;

};