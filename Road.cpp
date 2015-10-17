#include "Road.h"

void Road::draw(VSShaderLib shader, GLint pvm_uniformId,
	GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId) {

	//Draw Road
	loadMesh(_meshes.at(1), shader);
	pushMatrix(MODEL);

	//top
	scale(MODEL, 7.0f, 0.51f, 1.0f);
	translate(MODEL, -0.5f, 0.0f, -3.5f);

	renderMesh(_meshes.at(1), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	//bottom
	loadMesh(_meshes.at(1), shader);
	pushMatrix(MODEL);

	scale(MODEL, 7.0f, 0.51f, 1.0f);
	translate(MODEL, -0.5f, 0.0f, 2.6f);

	renderMesh(_meshes.at(1), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	//left
	loadMesh(_meshes.at(1), shader);
	pushMatrix(MODEL);

	scale(MODEL, 1.0f, 0.51f, 7.0f);
	translate(MODEL, -3.5f, 0.0f, -0.5f);

	renderMesh(_meshes.at(1), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	//right
	loadMesh(_meshes.at(1), shader);
	pushMatrix(MODEL);

	scale(MODEL, 1.0f, 0.51f, 7.0f);
	translate(MODEL, 2.5f, 0.0f, -0.5f);

	renderMesh(_meshes.at(1), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	//Start Marker
	loadMesh(_meshes.at(2), shader);
	pushMatrix(MODEL);

	scale(MODEL, 0.1f, 0.5f, 1.0f);
	translate(MODEL, 0.0f, 0.03f, 2.6f);

	renderMesh(_meshes.at(2), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

}