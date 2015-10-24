#pragma once
#include "StaticObject.h"

class Butter : public StaticObject {

private:
	float _xPosition;

public:

	void draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId);
};