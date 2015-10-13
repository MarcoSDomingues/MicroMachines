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

Vector3 Car::getDirection() {
	return _direction;
}

void Car::turn(int rotation) {
	_angle = (_angle+rotation % 360 + 360) % 360;

	float a = _angle * 3.14 / 180;

	float x = 3 * sin(a);
	float z = 3 * cos(a);
	_direction.set(x, 0, z);
}

void Car::draw(float x, float y, float z,
	VSShaderLib shader,
	GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId) {

	_position.set(x, y, z);

	pushMatrix(MODEL);
	translate(MODEL, x, y, z);
	rotate(MODEL, _angle, 0, 1, 0);
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

	_speed += _acceleration * delta_t;
	
	if (_acceleration > 0) {
		if (_speed > MAX_SPEED)
			_speed = MAX_SPEED;
	}
	else {
		if (_speed < -MAX_SPEED)
			_speed = -MAX_SPEED;
	}

	std::cout << _speed << std::endl;

	float x = _position.getX() + _direction.getX() * _speed * delta_t;
	float z = _position.getZ() + _direction.getZ() * _speed * delta_t;

	_position.set(x, 0.45, z);
}