#pragma once
#include "StaticObject.h"

class Pause : public StaticObject {
public:
	void draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId);
};