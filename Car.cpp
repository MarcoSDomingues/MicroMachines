#include "Car.h"

void Car::setAcceleration(float acceleration) {
	_acceleration = acceleration;
}

void Car::setSpeed(float speed) {
	_speed = speed;
}

float Car::getAcceleration() {
	return _acceleration;
}

float Car::getSpeed() {
	return _speed;
}

void Car::setDirection(float x, float y, float z) {
	_direction.set(x, y, z);
}

void Car::draw(float x, float y, float z,
	VSShaderLib shader,
	GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId) {
	pushMatrix(MODEL);
	translate(MODEL, x, y, z);
	rotate(MODEL, 90, 0, 1, 0);
	scale(MODEL, 0.2f, 0.2f, 0.2f);

	loadMesh(_meshes.at(0), shader);

	pushMatrix(MODEL);
	translate(MODEL, 0.65f, 1.1f, 0.65f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, 0.65f, 1.1f, -0.65f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, -0.65f, 1.1f, 0.65f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, -0.65f, 1.1f, -0.65f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	//DRAWBIGCUBE

	loadMesh(_meshes.at(1), shader);
	pushMatrix(MODEL);
	translate(MODEL, -0.5, 1, -0.8);
	scale(MODEL, 1.0f, 0.5f, 2.0f);
	//rotate(MODEL, 90, 90, 0, 0);
	renderMesh(_meshes.at(1), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);

	//DRAWSMALLCUBE

	loadMesh(_meshes.at(2), shader);
	pushMatrix(MODEL);
	translate(MODEL, -0.45, 1, -0.5);
	scale(MODEL, 0.9f, 0.9f, 0.9f);
	//rotate(MODEL, 90, 90, 0, 0);
	renderMesh(_meshes.at(2), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(MODEL);
	popMatrix(MODEL);
}

void Car::update(double delta_t) {

	float x = _speed * delta_t;
	float z = _speed * delta_t;
	_position.set(x, 0.0f, z);
}