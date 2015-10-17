#pragma once
#include "GameObject.h"

class Orange : public GameObject {

private:
	float _speed = 0.0f;
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

	void draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId);
	void update(double delta_t);
};
