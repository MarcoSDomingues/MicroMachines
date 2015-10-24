#pragma once

#include "StaticObject.h"

class Cheerio : public StaticObject {

public:

	void draw(VSShaderLib shader, GLint pvm_uniformId,
		GLint vm_uniformId, GLint normal_uniformId);

	void cheerio(MatrixTypes aType, float x, float y, float z, VSShaderLib shader, GLint pvm_uniformId,
		GLint vm_uniformId, GLint normal_uniformId);

};