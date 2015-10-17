#include "Table.h"

void Table::draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId)
{
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	scale(MODEL, 9.0f, 0.5f, 9.0f);
	translate(MODEL, _position.getX(), _position.getY(), _position.getZ());

	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);
}
