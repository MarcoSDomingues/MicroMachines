#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Vector3.h"

class Entity {

protected:

	Vector3 _position;

public:

	Entity();
	~Entity();

	void setPosition(float x, float y, float z);
	void setPosition(const Vector3& vec);

	Vector3 getPosition();

};