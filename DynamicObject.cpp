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