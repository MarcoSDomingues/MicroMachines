#pragma once
#include "GameObject.h"

class Car : public GameObject {
public:
	void draw(float x, float y, float z,
		VSShaderLib shader,
		GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId);
};