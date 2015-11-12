#include "Cup.h"

void Cup::draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId)
{
	pushMatrix(MODEL);

	translate(MODEL, _position.getX(), _position.getY(), _position.getZ());

	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);
	scale(MODEL,0.5f, 10.0f, 0.5f);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	loadMesh(_meshes.at(1), shader);
	pushMatrix(MODEL);
	translate(MODEL, 0.0,-0.5,0.0);
	scale(MODEL, 0.5f, 0.01f, 0.5f);
	renderMesh(_meshes.at(1), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	popMatrix(MODEL);
}

