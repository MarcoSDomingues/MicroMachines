#pragma once

#include "GameObject.h"
#include "Vector4.h"

class DynamicObject : public GameObject {

protected:

	float _speed = 0;
	float _acceleration = 0.0f;
	Vector4 _checkBox;

public:

	void setSpeed(float acceleration);
	void setAcceleration(float speed);
	float getSpeed();
	float getAcceleration();
	Vector4 getCheckBox();
	void setCheckBox(double x, double y, double z, double w);
	void checkCollision(double x, double y, double z);

	virtual void update(double delta_t) = 0;

};