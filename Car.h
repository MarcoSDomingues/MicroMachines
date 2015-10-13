#pragma once
#include "GameObject.h"

class Car : public GameObject {

private:
	Vector3 _direction = Vector3(-1.0f, 0.0f, 0.0f);
	float _speed = 0.005f;
	float _acceleration = 0.0f;

public:

	void setSpeed(float acceleration);
	void setAcceleration(float speed);
	
	float getSpeed();
	float getAcceleration();

	void setDirection(float x, float y, float z);

	void draw(float x, float y, float z,
		VSShaderLib shader,
		GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId);
	void update(double delta_t);
};