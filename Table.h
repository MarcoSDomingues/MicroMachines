#pragma once
#include "GameObject.h"

class Table : public GameObject {

private:
	float _xPosition;

public:

	void draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId);
};