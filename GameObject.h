#pragma once

#include <iostream>
#include "Vector3.h"

class GameObject {

protected:

	Vector3 _position;

public:

	GameObject();
	~GameObject();

	void setPosition(float x, float y, float z);

	Vector3 getPosition();

	virtual void draw() = 0;
	virtual void update(double delta_t) = 0;
};