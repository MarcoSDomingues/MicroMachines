#include "Car.h"

Vector3 Car::getDirection() {
	return _direction;
}

void Car::setInitialDirection() {
	_angle = -90;
}

float Car::getAngle() {
	return _angle;
}

void Car::kill() {
	setPosition(0.0f, 0.45f, 2.8f);
	setSpeed(0);
	setAcceleration(0);
	setInitialDirection();
}

void Car::draw(VSShaderLib shader, GLint pvm_uniformId,
	GLint vm_uniformId, GLint normal_uniformId) {
	float x = _position.getX();
	float y = _position.getY();
	float z = _position.getZ();

	pushMatrix(MODEL);
	translate(MODEL, x, y, z);
	rotate(MODEL, _angle, 0, 1, 0);
	scale(MODEL, 0.2f, 0.2f, 0.2f);

	loadMesh(_meshes.at(0), shader);

	pushMatrix(MODEL);
	translate(MODEL, 0.65f, 1.1f, 0.65f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, 0.65f, 1.1f, -0.65f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, -0.65f, 1.1f, 0.65f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	pushMatrix(MODEL);
	translate(MODEL, -0.65f, 1.1f, -0.65f);
	scale(MODEL, 0.15f, 0.15f, 0.15f);
	rotate(MODEL, 90, 0, 0, 90);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	//DRAWBIGCUBE

	loadMesh(_meshes.at(1), shader);
	pushMatrix(MODEL);
	translate(MODEL, -0.5, 1, -0.8);
	scale(MODEL, 1.0f, 0.5f, 2.0f);
	//rotate(MODEL, 90, 90, 0, 0);
	renderMesh(_meshes.at(1), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	//DRAWSMALLCUBE

	loadMesh(_meshes.at(2), shader);
	pushMatrix(MODEL);
	translate(MODEL, -0.45, 1, -0.5);
	scale(MODEL, 0.9f, 0.9f, 0.9f);
	//rotate(MODEL, 90, 90, 0, 0);
	renderMesh(_meshes.at(2), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);
	popMatrix(MODEL);
}

void Car::update(double delta_t) {

	float oldSpeed = _speed;

	_speed += _acceleration * delta_t;

	float stop = (oldSpeed / abs(oldSpeed)) != (_speed / abs(_speed));

	if (_stopping && stop) {
		_acceleration = 0;
	}
	
	if (_speed > MAX_SPEED) _speed = MAX_SPEED;
	else if (_speed < -MAX_SPEED) _speed = -MAX_SPEED;

	float x = _position.getX() + _direction.getX() * _speed * delta_t;
	float z = _position.getZ() + _direction.getZ() * _speed * delta_t;

	_position.set(x, 0.35, z);

	//angle stuff
	_angle += _turning * ROTATION * delta_t;
	if (_angle > 360) _angle = 0;
	else if (_angle < 0) _angle = 360;
	float da = _angle * M_PI / 180;
	float dx = 3 * sin(da);
	float dz = 3 * cos(da);
	_direction.set(dx, 0, dz);

	float dir = 1;
	if (_speed < 0) {
		dir = -1;
	}
	dx /= 15 * dir;
	dz /= 15 * dir;

	float bb = 0.1; //bounding box half-width and half-height
	_checkBox.set(x + dx - bb, x + dx + bb, z + dz - bb, z + dz + bb);
}

void Car::accelerate() {
	_acceleration = ACCELERATION;
	_stopping = false;
}

void Car::stopForward() {
	if (_acceleration > 0) {
		_acceleration = ACCELERATION * -_speed / abs(_speed);
		_stopping = true;
	}
}

void Car::stopBack() {
	if (_acceleration < 0) {
		_acceleration = ACCELERATION * -_speed / abs(_speed);
		_stopping = true;
	}
}

void Car::reverse() {
	_acceleration = -ACCELERATION;
	_stopping = false;
}

void Car::left() {
	_turning = 1;
}

void Car::right() {
	_turning = -1;
}

void Car::stopLeft() {
	if (_turning == 1) _turning = 0;
}

void Car::stopRight() {
	if (_turning == -1) _turning = 0;
}
