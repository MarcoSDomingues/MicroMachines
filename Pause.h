#pragma once
#include "StaticObject.h"
#include "OrtogonalCamera.h"

class Pause : public StaticObject {
private:
	OrtogonalCamera* _camera;
public:
	Pause();
	void draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId);
};