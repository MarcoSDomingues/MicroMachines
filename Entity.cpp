#include "Entity.h"

Entity::Entity() {
}

Entity::~Entity() {}

Vector3 Entity::getPosition() {
	return _position;
}

void Entity::setPosition(float x, float y, float z) {
	_position.set(x, y, z);
}

void Entity::setPosition(const Vector3 &p) {
	_position = p;
}