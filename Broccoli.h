#pragma once
#include "StaticObject.h"

#define M_PI       3.14159265358979323846f

class Broccoli : public StaticObject {

private:
	float _xPosition;
	float _angle;
	Vector3 _dir;

public:

	void setAngle(float angle);
	void setDirection(float x, float y, float z);

	void draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId);
};