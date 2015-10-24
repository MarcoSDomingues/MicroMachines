#include "DynamicObject.h"

void DynamicObject::setAcceleration(float acceleration) {
	_acceleration = acceleration;
}

void DynamicObject::setSpeed(float speed) {
	_speed = speed;
}

float DynamicObject::getAcceleration() {
	return _acceleration;
}

float DynamicObject::getSpeed() {
	return _speed;
}

Vector4 DynamicObject::getCheckBox() {
	return _checkBox;
}

void DynamicObject::setCheckBox(double x, double y, double z, double w) {
	_checkBox.set(x, y, z, w);
}

void DynamicObject::checkCollision(double x, double y, double z) {
	_position.set(x, y, z);
	_checkBox.set(x - getCheckBox().getX(), x + getCheckBox().getY(), y - getCheckBox().getZ(), y + getCheckBox().getW());
}