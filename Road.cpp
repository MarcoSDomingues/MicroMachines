#include "Road.h"

void Road::draw(VSShaderLib shader, GLint pvm_uniformId,
	GLint vm_uniformId, GLint normal_uniformId) {

	//Draw Road
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	//
	translate(MODEL, -2.5f, 0.0f, -2.5f);
	rotate(MODEL, 90, 0, 1, 0);
	scale(MODEL, 1.0f, 0.51f, 5.0f);


	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	//bottom
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	translate(MODEL, -2.5f, 0.0f, 3.6f);
	rotate(MODEL, 90, 0, 1, 0);
	scale(MODEL, 1.0f, 0.51f, 5.0f);

	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	//left
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	translate(MODEL, -3.5f, 0.0f, -3.5f);
	scale(MODEL, 1.0f, 0.51f, 7.1f);


	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	//right
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	translate(MODEL, 2.5f, 0.0f, -3.5f);
	scale(MODEL, 1.0f, 0.51f, 7.1f);


	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	//Start Marker
	loadMesh(_meshes.at(1), shader);
	pushMatrix(MODEL);

	scale(MODEL, 0.1f, 0.5f, 1.0f);
	translate(MODEL, 0.0f, 0.03f, 2.6f);

	renderMesh(_meshes.at(1), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

}