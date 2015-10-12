#pragma once

#include <iostream>

class GameObject {

protected:

	float _x;
	float _y;
	float _z;

public:

	GameObject();
	~GameObject();

	void setPosition(float x, float y, float z);

	float getX();
	float getY();
	float getZ();

	virtual void draw() = 0;
	virtual void update(double delta_t) = 0;
};