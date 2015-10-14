#pragma once
#include "GameObject.h"

#define MAX_SPEED 0.001
# define ACCELERATION 0.0000005

class Car : public GameObject {

private:
	Vector3 _direction = Vector3(-1.0f, 0.0f, 0.0f);
	float _speed = 0;
	float _acceleration = 0.0f;
	int _angle = -90;
	bool _stopping = false;

public:

	void setSpeed(float acceleration);
	void setAcceleration(float speed);

	void accelerate();
	void stop();
	void reverse();
	
	float getSpeed();
	float getAcceleration();
	Vector3 getDirection();
	void turn(int rotation);

	void draw(float x, float y, float z,
		VSShaderLib shader,
		GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId);
	void update(double delta_t);
};