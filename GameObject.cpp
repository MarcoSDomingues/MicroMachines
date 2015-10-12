#include "GameObject.h"

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::setPosition(float x, float y, float z) {
	_x = x;
	_y = y;
	_z = z;
}

float GameObject::getX() {
	return _x;
}

float GameObject::getY() {
	return _y;
}

float GameObject::getZ() {
	return _z;
}