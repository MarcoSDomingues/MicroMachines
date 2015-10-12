#include "Vector3.h"

Vector3::Vector3() {
	Vector3(0, 0, 0);

}

Vector3::Vector3(double x, double y, double z) {
	_x = x;
	_y = y;
	_z = z;
}


Vector3::~Vector3()
{
}


double Vector3::getX() {
	return _x;
}

double Vector3::getY() {
	return _y;
}

double Vector3::getZ() {
	return _z;
}

void Vector3::set(double x, double y, double z) {
	_x = x;
	_y = y;
	_z = z;
}

Vector3 Vector3::operator=(const Vector3 &vec) {
	_x = vec._x;
	_y = vec._y;
	_z = vec._z;
	return *this;
}

Vector3 Vector3::operator*(double num) {
	double newX = _x*num;
	double newY = _y*num;
	double newZ = _z*num;

	return Vector3(newX, newY, newZ);

}

Vector3 Vector3::operator+(const Vector3& vec) {
	double newX = _x + vec._x;
	double newY = _y + vec._y;
	double newZ = _z + vec._z;

	Vector3 newVector(newX, newY, newZ);

	return newVector;
}

Vector3 Vector3::operator-(const Vector3& vec) {
	double newX = _x - vec._x;
	double newY = _y - vec._y;
	double newZ = _z - vec._z;

	return Vector3(newX, newY, newZ);
}