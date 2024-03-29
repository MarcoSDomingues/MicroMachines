#pragma once
#include "DynamicObject.h"

class Orange : public DynamicObject {

private:
	float _yRotation = 0.0f;
	float _xPosition;
	float _velocity;
	bool _delayDraw = false;
	float _speed_timer;
	int _speedInc = 1.0f;

public:

	void setSpeed(float speed);
	void setDelayDraw(bool delay);

	float getSpeed();
	bool getDelayDraw();

	void init();
	float randomVelocity();

	void draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId);
	void update(double delta_t);
};
