#include "Butter.h"

void Butter::draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId)
{
	_checkBox.set(_position.getX() - 0.5, _position.getX() + 0.5, _position.getZ() - 1.0, _position.getZ() + 1.0);

	pushMatrix(MODEL);

	translate(MODEL, _position.getX(), _position.getY(), _position.getZ());
	//rotate(MODEL, -45, 0, 1, 0);
	scale(MODEL, 0.7f, 0.7f, 0.7f);

	loadMesh(_meshes.at(0), shader);
	
	pushMatrix(MODEL);
	scale(MODEL, 1.0f, 0.5f, 1.2f);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, -0.1f, 0.5f, -0.1f);
	scale(MODEL, 1.2f, 0.1f, 1.4f);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	popMatrix(MODEL);
}
