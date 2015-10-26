#include "Cheerio.h"

void Cheerio::draw(VSShaderLib shader, GLint pvm_uniformId,
	GLint vm_uniformId, GLint normal_uniformId) {

	float bb = 0.1;
	_checkBox.set(_position.getX() - bb - 0.1, _position.getX() + bb - 0.1, _position.getZ() - bb, _position.getZ() + bb);

	cheerio(MODEL, _position.getX(), 0.52f, _position.getZ(), shader, pvm_uniformId, vm_uniformId, normal_uniformId);
}

void Cheerio::cheerio(MatrixTypes aType, float x, float y, float z, VSShaderLib shader, GLint pvm_uniformId,
	GLint vm_uniformId, GLint normal_uniformId) {

	//_checkBox.set(_position.getX() - 0.1, _position.getX() + 0.1, _position.getZ() - 1.0, _position.getZ() + 1.0);

	loadMesh(_meshes.at(0), shader);
	pushMatrix(aType);

	translate(aType, x, y, z);

	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(aType);

}