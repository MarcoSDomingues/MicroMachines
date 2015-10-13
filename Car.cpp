#include "Car.h"

void Car::draw(float x, float y, float z,
	VSShaderLib shader,
	GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId) {
	pushMatrix(MODEL);
	translate(MODEL, x, y, z);
	rotate(MODEL, 90, 0, 1, 0);
	scale(MODEL, 0.2f, 0.2f, 0.2f);

	loadMesh(_meshes.at(0), shader);

	pushMatrix(MODEL);
	translate(MODEL, x + 0.65f, y + 0.25f, z + 0.2f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, x - 0.65f, y + 0.25f, z + 0.2f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, x + 0.65f, y + 0.25f, z - 1.1f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, x - 0.65f, y + 0.25f, z - 1.1f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	//DRAWBIGCUBE

	loadMesh(_meshes.at(1), shader);
	pushMatrix(MODEL);
	translate(MODEL, x - 0.5f, y + 0.0f, z - 1.5f);
	scale(MODEL, 1.0f, 0.5f, 2.0f);
	//rotate(MODEL, 90, 90, 0, 0);
	renderMesh(_meshes.at(1), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	//DRAWSMALLCUBE

	loadMesh(_meshes.at(2), shader);
	pushMatrix(MODEL);
	translate(MODEL, x - 0.45f, y + 0.1f, z - 0.45f);
	scale(MODEL, 0.9f, 0.9f, 0.9f);
	//rotate(MODEL, 90, 90, 0, 0);
	renderMesh(_meshes.at(2), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);
	popMatrix(MODEL);
}