#include "Table.h"

void Table::draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId)
{
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	translate(MODEL, _position.getX(), _position.getY(), _position.getZ());
	scale(MODEL, 9.0f, 0.0000001f, 9.0f);

	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);
}
