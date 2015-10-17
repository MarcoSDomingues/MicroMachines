#pragma once
#include "DynamicObject.h"

#define MAX_SPEED 0.001
#define ACCELERATION 0.0000005
#define ROTATION 0.2

class Car : public DynamicObject {

private:

	Vector3 _direction = Vector3(-1.0f, 0.0f, 0.0f);
	float _angle = -90;
	bool _stopping = false;
	int _turning = 0; //1 is right, -1 is left, 0 is stopped

public:

	void accelerate();
	void stopForward();
	void stopBack();
	void reverse();

	void left();
	void right();
	void stopLeft();
	void stopRight();
	
	Vector3 getDirection();

	void draw(VSShaderLib shader, GLint pvm_uniformId,
		GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId);
	void update(double delta_t);
};