#include "GameObject.h"

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::setPosition(float x, float y, float z) {
	_position.set(x, y, z);
}

Vector3 GameObject::getPosition() {
	return _position;
}