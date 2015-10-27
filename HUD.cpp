#include "HUD.h"
void HUD::draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId)
{
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	translate(MODEL, _position.getX(), _position.getY(), _position.getZ());
	scale(MODEL, 3.0f, 0.5f, 1.0f);


	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);
}